//Thanks to https://github.com/JamesRandall/SimpleVoxelEngine/blob/master/voxelEngine/src/AABBTree.cpp

#include <iostream>
#include "Engine/Physics/AABBTree.h"

//#include <iostream>

namespace Engine::Physics {
    typedef int32_t NodeId;

    //this rect representation consumes less memory and is quicker for the operations
    struct AABB {
        float minX, minY;
        float maxX, maxY;
        float area;

        AABB() = default;

        AABB(float minX_, float minY_,
             float maxX_, float maxY_)
                : minX(minX_), minY(minY_),
                  maxX(maxX_), maxY(maxY_),
                  area((maxX_ - minX_) * (maxY_ - minY_)) {}
    };


    AABB rectToAABB(Math::Rect_d rect) {
        auto p1 = rect.minPoint();
        auto p2 = rect.maxPoint();
        return {
                (float) p1.x, (float) p1.y,
                (float) p2.x, (float) p2.y
        };
    }

    Math::Rect_d aabbToRect(AABB aabb) {
        return {
                {aabb.minX, aabb.minY},
                {aabb.maxX - aabb.minX, aabb.maxY - aabb.minY}
        };
    }

    inline AABB unite(AABB boxA, AABB boxB) {
        return AABB{
                std::min(boxA.minX, boxB.minX),
                std::min(boxA.minY, boxB.minY),
                std::max(boxA.maxX, boxB.maxX),
                std::max(boxA.maxY, boxB.maxY)
        };
    }

    inline bool doIntersect(AABB boxA, AABB boxB) {
        return boxA.maxX > boxB.minX && boxB.maxX > boxA.minX &&
               boxA.maxY > boxB.minY && boxB.maxY > boxA.minY;
    }

    float area(AABB box) {
        return (box.maxX - box.minX) * (box.maxY - box.minY);
    }

    constexpr NodeId NullNode = -1;

    struct Node {
        Node() {}

        AABB aabb;

        //only leaf nodes contain data
        //leaf nodes have no children
        union {
            struct {
                NodeId left = NullNode;
                NodeId right = NullNode;
            };
            void *data;
        };

        //removed node does not have a parent
        union {
            NodeId parent = NullNode;

            //if the node was removed from tree, it stores the next removed or not-yet-added node
            //to create a linked list of free nodes
            NodeId next;
        };

        bool leaf = true;
    };

    class AABBTree::Impl {
    public:
        void insert(void *data, Math::Rect_d rect) noexcept {
            auto nodeId = _allocateNode();
            _fillLeafWithData(nodeId, data, rectToAABB(rect));

            if (root == NullNode) {
                root = nodeId;
                return;
            }

            //doing allocation in the beginning to avoid dangling references
            NodeId holderId = _allocateNode();

            //vector might have been reallocated
            //so it is obligatory to extract reference here
            Node &node = nodes[nodeId];

            //looking for node to replace
            NodeId curId = root;
            while (!nodes[curId].leaf) {
                Node &cur = nodes[curId];

                NodeId leftId = cur.left;
                Node &left = nodes[leftId];

                NodeId rightId = cur.right;
                Node &right = nodes[rightId];

                auto combinedAabb = unite(cur.aabb, node.aabb);

                float newParentNodeCost = 2.0f * combinedAabb.area;
                float minimumPushDownCost = 2.0f * combinedAabb.area - cur.aabb.area;

                float costLeft;
                float costRight;

                if (left.leaf) {
                    costLeft = unite(node.aabb, left.aabb).area + minimumPushDownCost;
                } else {
                    costLeft = unite(node.aabb, left.aabb).area - left.aabb.area + minimumPushDownCost;
                }

                if (right.leaf) {
                    costRight = unite(node.aabb, right.aabb).area + minimumPushDownCost;
                } else {
                    costRight = unite(node.aabb, right.aabb).area - right.aabb.area + minimumPushDownCost;
                }

                //constructing new node right here instead of current
                if (newParentNodeCost < costLeft && newParentNodeCost < costRight)
                    break;
                curId = (costLeft < costRight ? leftId : rightId);
            }

            _insertLeafInPlace(nodeId, curId, holderId);

            _fixUpwards(holderId);
        }

        void erase(void *data, Math::Rect_d rect) noexcept {
            NodeId id = _findNode(data, rectToAABB(rect));

            if (id == NullNode)
                return;

            if (id == root) {
                root = NullNode;
            } else {
                Node &node = nodes[id];

                NodeId parentId = node.parent;
                Node &parent = nodes[node.parent];

                NodeId siblingId = (parent.left == id ? parent.right : parent.left);
                Node &sibling = nodes[siblingId];

                NodeId grandparentId = parent.parent;

                if (grandparentId == NullNode) {
                    sibling.parent = NullNode;
                    root = siblingId;
                } else {
                    Node &grandparent = nodes[grandparentId];
                    sibling.parent = grandparentId;
                    if (grandparent.left == parentId)
                        grandparent.left = siblingId;
                    else
                        grandparent.right = siblingId;
                }

                _deallocateNode(parentId);
            }

            _deallocateNode(id);
        }

        SmallVector<void *> query(Math::Rect_d rect) noexcept {
            SmallVector<void *> result;
            auto callback = [&result, this](NodeId id) -> bool {
                result.push_back(nodes[id].data);
                return false;
            };
            AABB aabb = rectToAABB(rect);
            _traverseOnlyLeaves(aabb, callback);
            return result;
        }

        void update(void *data, Math::Rect_d rect) noexcept {
            AABB aabb = rectToAABB(rect);
            NodeId id = _findNode(data, aabb);

            if (id == NullNode)
                return;

            Node &node = nodes[id];

            const static float reinsertCriteria = 0.7f;

            AABB newAABB = unite(node.aabb, aabb);
            float newArea = newAABB.area;

            if (newArea * reinsertCriteria > aabb.area) {
                erase(data, rect);
                insert(data, rect);
            } else {
                node.aabb = newAABB;
                _fixUpwards(node.parent);
            }
        }

        void visualize(AABBTreeVisualizer &viz) {
            auto callback = [this, &viz](NodeId id) -> bool {
                viz.render(aabbToRect(nodes[id].aabb));
                return false;
            };
            if (root != NullNode)
                _traverseAllNodes(nodes[root].aabb, callback);
        }

        void clear() noexcept {
//            nodes.clear();
            for (size_t i = 0; i < nodes.size(); ++i)
                _deallocateNode(i);
            root = NullNode;
        }

    private:
        NodeId root = NullNode;
        NodeId nextNode = NullNode;
        int allocatedNodes = 0;
        std::vector<Node> nodes;
        //making it global not to loose capacity
        std::vector<NodeId> traverseStack;

        inline void _insertLeafInPlace(NodeId nodeId, NodeId siblingId, NodeId holderId) {
            //holder becomes parent of node and its sibling

            Node &node = nodes[nodeId];
            Node &holder = nodes[holderId];
            Node &sibling = nodes[siblingId];

            //replacing
            holder.leaf = false;
            auto pId = sibling.parent;
            holder.parent = pId;

            holder.left = nodeId;
            holder.right = siblingId;

            node.parent = holderId;
            sibling.parent = holderId;

            //fixing parent links
            if (holder.parent == NullNode) {
                root = holderId;
            } else {
                auto &parent = nodes[holder.parent];
                if (parent.left == siblingId)
                    parent.left = holderId;
                else
                    parent.right = holderId;
            }
        }

        inline void _fillLeafWithData(NodeId id, void *data, AABB aabb) {
            auto &node = nodes[id];

            node.leaf = true;
            node.parent = NullNode;
            node.data = data;
            node.aabb = aabb;
        }

        NodeId _allocateNode() noexcept {
            ++allocatedNodes;
            if (nextNode == NullNode) {
                nodes.emplace_back();
                return nodes.size() - 1;
            } else {
                auto id = nextNode;
                nextNode = nodes[id].next;
                return id;
            }
        }

        void _deallocateNode(NodeId id) noexcept {
            --allocatedNodes;
            auto &node = nodes[id];
            node.leaf = true;
            node.next = nextNode;
            nextNode = id;
        }

        NodeId _findNode(void *data, AABB aabb) noexcept {
            NodeId result = NullNode;
            auto callback = [&result, this, data](NodeId id) -> bool {
                if (nodes[id].data == data) {
                    result = id;
                    return true;
                }
                return false;
            };
            _traverseOnlyLeaves(aabb, callback);
            return result;
        }

        //Strange behaviour of std::vector appeared here
        //https://stackoverflow.com/questions/63535426/stdvector-reallocations-regardless-reserved-capacity-when-used-as-a-stack
#if DEBUG_LEVEL == 0 || !defined(DEBUG_LEVEL)
        template<typename Callback>
        void _traverseAllNodes(AABB aabb, Callback &cb) {
            _traverse(aabb, cb, false);
        }

        template<typename Callback>
        void _traverseOnlyLeaves(AABB aabb, Callback &cb) {
            _traverse(aabb, cb, true);
        }
#else
        template<typename Callback>
        void _traverseAllNodes(AABB aabb, Callback &cb) {
            _traverseRecursive(root, aabb, cb, false);
        }

        template<typename Callback>
        void _traverseOnlyLeaves(AABB aabb, Callback &cb) {
            _traverseRecursive(root, aabb, cb, true);
        }
#endif

        //expects callback to be a bool(NodeId)
        //if callback returns true, the traverse is aborted
        template<typename Callback>
        void _traverse(AABB aabb, Callback &cb, bool callOnlyOnLeaves = false) {
            if (root == NullNode)
                return;
//            std::cout << traverseStack.capacity() << "\n";
            traverseStack.push_back(root);
            while (!traverseStack.empty()) {
                NodeId curId = traverseStack.back();
                Node &cur = nodes[curId];
                traverseStack.pop_back();
//                std::cout << traverseStack.size() << " " << traverseStack.capacity() << "\n";
                if (doIntersect(cur.aabb, aabb)) {
                    if ((!callOnlyOnLeaves || cur.leaf) && cb(curId))
                        break;
                    if (!cur.leaf) {
                        traverseStack.push_back(cur.left);
                        traverseStack.push_back(cur.right);
                    }
                }
            }
            traverseStack.clear();
        }

        template<typename Callback>
        bool _traverseRecursive(NodeId curId, AABB aabb, Callback &cb,
                                bool callOnlyOnLeaves = false) {
            if (curId == NullNode)
                return false;
            Node &cur = nodes[curId];
            if (doIntersect(cur.aabb, aabb)) {
                if ((!callOnlyOnLeaves || cur.leaf) && cb(curId))
                    return true;
                if (!cur.leaf) {
                    if (!_traverseRecursive(cur.left, aabb, cb, callOnlyOnLeaves))
                        return _traverseRecursive(cur.right, aabb, cb, callOnlyOnLeaves);
                    else
                        return true;
                }
            }
            return false;
        }

        void _fixUpwards(NodeId id) noexcept {
            while (id != NullNode) {
                auto &node = nodes[id];
                auto &left = nodes[node.left];
                auto &right = nodes[node.right];

                node.aabb = unite(left.aabb, right.aabb);

                id = node.parent;
            }
        }
    };


    AABBTree::AABBTree() : _impl(new Impl()) {}

    void AABBTree::insert(void *data, Math::Rect_d aabb) noexcept {
        _impl->insert(data, aabb);
    }

    void AABBTree::insert(std::pair<void *, Math::Rect_d> pair) noexcept {
        _impl->insert(pair.first, pair.second);
    }

    void AABBTree::erase(void *data, Math::Rect_d aabb) noexcept {
        _impl->erase(data, aabb);
    }

    void AABBTree::erase(std::pair<void *, Math::Rect_d> pair) noexcept {
        _impl->erase(pair.first, pair.second);
    }

    void AABBTree::update(void *data, Math::Rect_d rect) noexcept {
        _impl->update(data, rect);
    }

    SmallVector<void *> AABBTree::query(Math::Rect_d aabb) noexcept {
        return _impl->query(aabb);
    }

    void AABBTree::visualize(AABBTreeVisualizer &visualizer) {
        _impl->visualize(visualizer);
    }

    void AABBTree::clear() noexcept {
        _impl->clear();
    }

    AABBTree::~AABBTree() = default;
}