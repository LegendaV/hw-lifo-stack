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

        Node(Node* last, unsigned char* data, const std::size_t dataSize)
        {
            this->count = 1;
            if (last != nullptr)
            {
                this->count = last->count + 1;
            }
            this->dataSize = dataSize;
            this->last = last;
            this->data = data;
        }

        ~Node()
        {
            delete data;
        }
    };

    static Handle stackCount = 0;
    static std::unordered_map<Handle, Node*> stackDict;

    Handle create()
    {
        stackDict[stackCount] = nullptr;
        return stackCount++;
    }

    bool valid(const Handle handle)
    {
        return stackDict.find(handle) != stackDict.end();
    }

    void destroy(const Handle handle)
    {
        if (valid(handle))
        {
            Node* current = stackDict.find(handle)->second;
            while (current != nullptr)
            {
                Node* prev = current->last;
                delete current;
                current = prev;
            }
            stackDict.erase(handle);
        }
    }

    std::size_t count(const Handle handle)
    {
        if (!valid(handle))
        {
            return 0u;
        }

        Node* head = stackDict.find(handle)->second;
        if (head == nullptr)
        {
            return 0u;
        }

        return head->count;
    }

    void push(const Handle handle, const void* const data, const std::size_t size)
    {
        const auto stack = stackDict.find(handle);
        if (stack == stackDict.end() || data == nullptr || size <= 0u)
        {
            return;
        }

        Node* lastHead = stack->second;

        unsigned char* buf = new unsigned char[size];
        std::memcpy(buf, data, size);

        Node* newNode = new Node(lastHead, buf, size);
        stackDict[handle] = newNode;
    }

    std::size_t pop(const Handle handle, void* const data, const std::size_t size)
    {
        if (!valid(handle) || data == nullptr || size <= 0u)
        {
            return 0u;
        }

        const Node* head = stackDict.find(handle)->second;
        if (head == nullptr)
        {
            return 0u;
        }

        stackDict[handle] = head->last;

        const std::size_t copySize = std::min(size, head->dataSize);
        std::memcpy(data, head->data, copySize);

        delete head;
        return copySize;
    }
} // namespace stack