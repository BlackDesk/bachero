//Info:
//https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-oriented-rigid-bodies--gamedev-8032
//https://github.com/erincatto/box2d-lite
//https://github.com/erincatto/box2d-lite/blob/master/docs/GDC2006_Catto_Erin_PhysicsTutorial.pdf
//Thanks to Randy Gall, Erin Catto and Alexander Krivokrysenko!

#ifndef BACHERO_ENGINE_PHYSICS_MANIFOLD
#define BACHERO_ENGINE_PHYSICS_MANIFOLD

#include "Engine/Physics/ColliderComponent.h"
#include "Engine/Physics/RigidBodyComponent.h"
#include "Engine/ECS/ECS.h"

namespace Engine::Physics {
    struct Contact {
        std::size_t feature = 0;
        Math::Vector2f position;
        Math::Vector2f normal;
        float separation = 0.0f;

        float Pn = 0.0f; //normal impulse (to prevent collisions)
        float Pt = 0.0f; //tangent impulse (to apply friction)
        float massNormal = 0.0f;
        float massTangent = 0.0f;
        float bias = 0.0f;
    };

    //needed to store pairs
    class ManifoldKey {
    public:
        ManifoldKey(ECS::Entity *ent1, ECS::Entity *ent2)
                : bodyA(ent1 <= ent2 ? ent1 : ent2),
                  bodyB(ent1 <= ent2 ? ent2 : ent1) {}

        bool operator<(const ManifoldKey &other) const {
            if (bodyA != other.bodyA)
                return bodyA < other.bodyA;
            else
                return bodyB < other.bodyB;
        }

        bool operator==(const ManifoldKey &other) const {
            return bodyA == other.bodyA && bodyB == other.bodyB;
        }

        bool operator!=(const ManifoldKey &other) const {
            return !(*this == other);
        }

        const ECS::Entity *bodyA = nullptr;
        const ECS::Entity *bodyB = nullptr;
    };

    class Manifold {
    public:
        const ECS::Entity *bodyA = nullptr;
        const ECS::Entity *bodyB = nullptr;

        Manifold() = default;

        Manifold(ECS::Entity *A, ECS::Entity *B)
                : bodyA(A), bodyB(B) {
            rigidA = bodyA->getComponent<RigidBodyComponent>();
            rigidB = bodyB->getComponent<RigidBodyComponent>();

            colliderA = bodyA->getComponent<ColliderComponent>();
            colliderB = bodyB->getComponent<ColliderComponent>();

            dt = DeltaTime::get();
            friction = sqrtf(rigidA->friction * rigidB->friction);
        }

        bool detectAndCalcCollision() {
            Contact oldContacts[2] = {contacts[0], contacts[1]};
            std::size_t oldContactsNum = contactsNum;
            contactsNum = 0;

            Math::Vector2f cornersA[4], cornersB[4];
            colliderA->getCorners(cornersA);
            colliderB->getCorners(cornersB);

            Math::Vector2f normalsA[4], normalsB[4];
            colliderA->getNormals(normalsA);
            colliderB->getNormals(normalsB);

            // Check for a separating axis with A's face planes
            std::size_t faceA;
            float penetrationA = findAxisOfLeastPenetration(faceA, normalsA, cornersA, cornersB);
            if (penetrationA >= 0.0f)
                return false;

            // Check for a separating axis with B's face planes
            std::size_t faceB;
            float penetrationB = findAxisOfLeastPenetration(faceB, normalsB, cornersB, cornersA);
            if (penetrationB >= 0.0f)
                return false;

            std::size_t referenceIndex;
            bool flip;
            Math::Vector2f *refCorners, *refNormals;
            Math::Vector2f *incCorners, *incNormals;

            if (biasGreaterThan(penetrationA, penetrationB)) {
                refCorners = cornersA, refNormals = normalsA;
                incCorners = cornersB, incNormals = normalsB;
                referenceIndex = faceA;
                flip = false;
            } else {
                refCorners = cornersB, refNormals = normalsB;
                incCorners = cornersA, incNormals = normalsA;
                referenceIndex = faceB;
                flip = true;
            }

            //two points of the edge
            Math::Vector2f incidentFace[2];
            std::size_t incidentIndex;
            findIncidentFace(refCorners, refNormals, incCorners, incNormals,
                             referenceIndex, incidentFace, incidentIndex);

            //setup reference face vertices
            Math::Vector2f v1 = refCorners[referenceIndex];
            Math::Vector2f v2 = refCorners[(referenceIndex + 1) % 4];

            //calculate reference face side normal in world space
            Math::Vector2f sidePlaneNormal = (v2 - v1);
            sidePlaneNormal.normalize();

            //orthogonalize
            Math::Vector2f refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

            // ax + by = c
            // c is distance from origin
            float refC = refFaceNormal.dotProduct(v1);
            float negSide = -sidePlaneNormal.dotProduct(v1);
            float posSide = sidePlaneNormal.dotProduct(v2);

            // Clip incident face to reference face side planes
            if (clip(-sidePlaneNormal, negSide, incidentFace) < 2)
                return false;

            if (clip(sidePlaneNormal, posSide, incidentFace) < 2)
                return false;

            //flip
            auto cNormal = (flip ? -refFaceNormal : refFaceNormal);

            //keep points behind reference face
            // clipped points behind reference face

            contactsNum = 0;
            for (std::size_t i = 0; i < 2; ++i) {
                float separation = refFaceNormal.dotProduct(incidentFace[i]) - refC;
                if (separation <= 0.0f) {
                    auto &contact = contacts[contactsNum];
                    contact.feature = incidentIndex + i + 4 * flip;
                    contact.position = incidentFace[i];
                    contact.normal = cNormal;
                    contact.separation = -separation;
                    ++contactsNum;
                }
            }

//            mergeContacts(oldContacts, oldContactsNum);

            return contactsNum > 0;
        }

        void preStep(float _dt) {
            dt = _dt;
            invDt = (dt > 0.0f ? 1.0f / dt : 0.0f);

            for (int i = 0; i < contactsNum; ++i) {
                Contact &c = contacts[i];

                Math::Vector2f r1 = c.position - colliderA->getAnchorPoint();
                Math::Vector2f r2 = c.position - colliderB->getAnchorPoint();

                preCalcNormal(c, r1, r2);
                preCalcTangent(c, r1, r2);
                preCalcBias(c);

                Math::Vector2f tangent = cross(c.normal, 1.0f);

                //accumulating impulses to approach true impulse
                //apply normal + friction impulse
                Math::Vector2f P = c.Pn * c.normal + c.Pt * tangent;

                rigidA->applyImpulse(-P, r1);
                rigidB->applyImpulse(P, r2);
            }
        }

        void resolve() {
            if (rigidA->invMass + rigidA->invMass == 0)
                return;

            for (std::size_t i = 0; i < contactsNum; ++i) {
                Contact &c = contacts[i];

                applyNormalImpulses(c);
                applyTangentImpulses(c);
            }
        }

    private:
        void preCalcNormal(Contact &c, Math::Vector2f r1, Math::Vector2f r2) {
            // Precompute normal mass, tangent mass, and bias.
            float rn1 = r1.dotProduct(c.normal);
            float rn2 = r2.dotProduct(c.normal);
            float kNormal = rigidA->invMass + rigidB->invMass;
            kNormal += rigidA->invI * (r1.dotProduct(r1) - rn1 * rn1) +
                       rigidB->invI * (r2.dotProduct(r2) - rn2 * rn2);
            c.massNormal = (kNormal > 0.0f ? 1.0f / kNormal : 0.0f);
        }

        void preCalcTangent(Contact &c, Math::Vector2f r1, Math::Vector2f r2) {
            Math::Vector2f tangent = cross(c.normal, 1.0f);
            float rt1 = r1.dotProduct(tangent);
            float rt2 = r2.dotProduct(tangent);
            float kTangent = rigidA->invMass + rigidB->invMass;
            kTangent += rigidA->invI * (r1.dotProduct(r1) - rt1 * rt1) +
                        rigidB->invI * (r2.dotProduct(r2) - rt2 * rt2);
            c.massTangent = (kTangent > 0.0 ? 1.0f / kTangent : 0.0f);
        }

        void preCalcBias(Contact &c) {
            const float k_allowedPenetration = 0.00f;
            const float k_biasFactor = 0.2f;
            c.bias = -k_biasFactor * invDt * std::min(0.0f, -abs(c.separation) + k_allowedPenetration);
        }

        void applyNormalImpulses(Contact &c) {
            Math::Vector2f r1 = c.position - colliderA->getAnchorPoint();
            Math::Vector2f r2 = c.position - colliderB->getAnchorPoint();

            //relative velocity at contact
            Math::Vector2f dv = rigidB->velocity + cross(rigidB->angularVelocity, r2)
                                - rigidA->velocity - cross(rigidA->angularVelocity, r1);

            //compute normal impulse
            float vn = dv.dotProduct(c.normal);

            float dPn = c.massNormal * (-vn + c.bias);

            //clamp the accumulated impulse
            float Pn0 = c.Pn;
            c.Pn = std::max(Pn0 + dPn, 0.0f);
            dPn = c.Pn - Pn0;

            //apply contact impulse
            Math::Vector2f Pn = dPn * c.normal;

            rigidA->applyImpulse(-Pn, r1);
            rigidB->applyImpulse(Pn, r2);
        }

        void applyTangentImpulses(Contact &c) {
            Math::Vector2f r1 = c.position - colliderA->getAnchorPoint();
            Math::Vector2f r2 = c.position - colliderB->getAnchorPoint();

            //relative velocity at contact
            Math::Vector2f dv = rigidB->velocity + cross(rigidB->angularVelocity, r2)
                                - rigidA->velocity - cross(rigidA->angularVelocity, r1);

            Math::Vector2f tangent = cross(c.normal, 1.0f);
            float vt = dv.dotProduct(tangent);
            float dPt = c.massTangent * (-vt);

            // Compute friction impulse
            float maxPt = friction * c.Pn;

            // Clamp friction
            float oldTangentImpulse = c.Pt;
            c.Pt = clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
            dPt = c.Pt - oldTangentImpulse;

            // Apply contact impulse
            Math::Vector2f Pt = dPt * tangent;

            rigidA->applyImpulse(-Pt, r1);
            rigidB->applyImpulse(Pt, r2);
        }

        static Math::Vector2f cross(Math::Vector2f a, float s) {
            return Math::Vector2f(s * a.y, -s * a.x);
        }

        static Math::Vector2f cross(float s, Math::Vector2f a) {
            return Math::Vector2f(-s * a.y, s * a.x);
        }

        static float clamp(float val, float min, float max) {
            return std::min(std::max(val, min), max);
        }

        float findAxisOfLeastPenetration(std::size_t &faceIndex,
                                         Math::Vector2f normalsA[],
                                         Math::Vector2f cornersA[],
                                         Math::Vector2f cornersB[]) {
            float bestDistance = -FLT_MAX;
            std::size_t bestIndex;

            for (std::size_t i = 0; i < 4; ++i) {
                Math::Vector2f n = normalsA[i];

                Math::Vector2f s = colliderB->getSupportPoint(cornersB, -n);

                Math::Vector2f v = cornersA[i];

                //signed distance between a line and a point
                float d = n.dotProduct(s - v);

                //distance relaxation
                if (d > bestDistance) {
                    bestDistance = d;
                    bestIndex = i;
                }
            }

            faceIndex = bestIndex;
            return bestDistance;
        }

        std::size_t clip(Math::Vector2f n, float c, Math::Vector2f *face) {
            std::size_t sp = 0;
            Math::Vector2f out[2] = {
                    face[0],
                    face[1]
            };

            //retrieve distances from each endpoint to the line
            // d = ax + by - c
            float d1 = n.dotProduct(face[0]) - c;
            float d2 = n.dotProduct(face[1]) - c;

            //if negative (behind plane) clip
            if (d1 <= 0.0f) out[sp++] = face[0];
            if (d2 <= 0.0f) out[sp++] = face[1];

            //if the points are on different sides of the plane
            // less than to ignore -0.0f
            if (d1 * d2 < 0.0f) {
                //push intersection point
                float alpha = d1 / (d1 - d2);
                out[sp] = face[0] + alpha * (face[1] - face[0]);
                ++sp;
            }

            //assign our new converted values
            face[0] = out[0];
            face[1] = out[1];

            assert(sp != 3);

            return sp;
        }

        static bool biasGreaterThan(float a, float b) {
            const float k_biasRelative = 0.95f;
            const float k_biasAbsolute = 0.01f;
            return a >= b * k_biasRelative + a * k_biasAbsolute;
        }

        void findIncidentFace(Math::Vector2f refContacts[], Math::Vector2f refNormals[],
                              Math::Vector2f incContacts[], Math::Vector2f incNormals[],
                              std::size_t referenceIndex, Math::Vector2f incFace[],
                              std::size_t &incidentIndex) {
            Math::Vector2f referenceNormal = refNormals[referenceIndex];

            // Find most anti-normal face on incident polygon
            std::size_t incidentFace = 0;
            float minDot = FLT_MAX;
            for (std::size_t i = 0; i < 4; ++i) {
                float dot = referenceNormal.dotProduct(incNormals[i]);
                if (dot < minDot) {
                    minDot = dot;
                    incidentFace = i;
                }
            }

            incidentIndex = incidentFace;
            // Assign face vertices for incidentFace
            incFace[0] = incContacts[incidentFace];
            incidentFace = (incidentFace + 1) % 4;
            incFace[1] = incContacts[incidentFace];
        }

        void mergeContacts(Contact oldContacts[], std::size_t oldContactsNum) {
            Contact mergedContacts[2];
            for (int i = 0; i < contactsNum; ++i)
                mergedContacts[i] = contacts[i];

            for (std::size_t i = 0; i < oldContactsNum; ++i) {
                Contact &cOld = oldContacts[i];
                int k = -1;
                for (int j = 0; j < contactsNum; ++j) {
                    Contact &cNew = contacts[j];
                    if (cNew.feature == cOld.feature) {
                        k = j;
                        break;
                    }
                }

                if (k > -1) {
                    Contact &c = mergedContacts[k];

                    c.Pn = cOld.Pn;
                    c.Pt = cOld.Pt;
                }
            }

            for (int i = 0; i < contactsNum; ++i)
                contacts[i] = mergedContacts[i];
        }

        float dt = 0.0f;
        float invDt = 0.0f;

        RigidBodyComponent *rigidA = nullptr;
        RigidBodyComponent *rigidB = nullptr;

        ColliderComponent *colliderA = nullptr;
        ColliderComponent *colliderB = nullptr;

        float friction = 0.0f;

    public:
        Contact contacts[2];
        std::size_t contactsNum = 0;
    };
}

namespace std {
    template<>
    struct std::hash<Engine::Physics::ManifoldKey> {
        inline std::size_t operator()(const Engine::Physics::ManifoldKey& key) const {
            auto first = hash<std::size_t>{}(reinterpret_cast<std::size_t>(key.bodyA));
            auto second = hash<std::size_t>{}(reinterpret_cast<std::size_t>(key.bodyB));
            //boost like hash
            std::size_t seed = first;
            return seed ^ (second + 0x9e3779b9 + (seed << 6) + (seed >> 2));
        }
    };
}

#endif //BACHERO_ENGINE_PHYSICS_MANIFOLD