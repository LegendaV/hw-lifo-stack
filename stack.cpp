#include "stack.hpp"

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <cstring>

namespace stack
{

    struct Node
    {
        std::size_t count;
        Node* last;
        std::size_t dataSize;
        unsigned char* data;

        Node()
        {
            count = 0;
            last = nullptr;
            dataSize = 0;
            data = nullptr;
        }

        Node(Node* last, unsigned char* data, const std::size_t dataSize)
        {
            this->count = last->count + 1;
            this->dataSize = dataSize;
            this->last = last;
            this->data = data;
        }
    };

    static Handle stackCount = 0;
    static std::unordered_map<Handle, Node> stackDict;

    Handle create()
    {
        const Node head;
        stackDict[stackCount] = head;
        return stackCount++;
    }

    void destroy(const Handle handle)
    {
        if (valid(handle))
        {
            stackDict.erase(handle);
        }
    }

    bool valid(const Handle handle)
    {
        return stackDict.find(handle) != stackDict.end();
    }

    std::size_t count(const Handle handle)
    {
        if (!valid(handle))
        {
            return 0u;
        }

        return stackDict.find(handle)->second.count;
    }

    void push(const Handle handle, const void* const data, const std::size_t size)
    {
        const auto stack = stackDict.find(handle);
        if (stack == stackDict.end())
        {
            return;
        }

        Node lastHead = stack->second;

        unsigned char* buf = new unsigned char[size];
        std::memcpy(buf, data, size);

        const Node newNode = Node(&lastHead, buf, size);
        stackDict[handle] = newNode;
    }

    std::size_t pop(const Handle handle, void* const data, const std::size_t size)
    {
        if (!valid(handle))
        {
            return 0u;
        }

        const Node head = stackDict.find(handle)->second;
        if (head.last == nullptr)
        {
            return 0u;
        }

        stackDict[handle] = *head.last;

        const std::size_t copySize = std::min(size, head.dataSize);
        std::memcpy(data, head.data, copySize);

        return copySize;
    }

} // namespace stack