
#pragma once

#include <cassert>
#include <limits>
#include <vector>
#include <optional>
#include <utils/VectorHelper.h>

    template<class VType>
    class AutoIndexer
    {
    public:
        static constexpr std::size_t InvalidIndex = std::numeric_limits<std::size_t>::max();
    protected:
        std::vector<std::optional<VType>> m_vector;
        std::size_t m_items{0};
        std::size_t m_firstFreeSlot{InvalidIndex};
        std::size_t m_freeSlots{0};

        bool m_unique{false};

        void updateFirstFreeSlot()
        {
            assert(m_freeSlots > 0);
            --m_freeSlots;
            if (m_freeSlots == 0)
            {
                m_firstFreeSlot = InvalidIndex;
                return;
            }
            for(std::size_t i = m_firstFreeSlot + 1; i < m_vector.size(); i++)
                if (!m_vector[i].has_value())
                {
                    m_firstFreeSlot = i;
                    return;
                }
            assert(("Cannot find free slot, while its number is not zero!", false));
        }
        void updateFreeSlotsNumber() {
            for (std::size_t i = m_firstFreeSlot; i < m_vector.size(); i++) {
                m_freeSlots = 0;
                if (!m_vector[i].has_value())  ++m_freeSlots;
            }
        }
    public:
        std::size_t firstFreeSlot() const { return m_firstFreeSlot; }
        std::size_t freeSlots() const { return m_freeSlots; }
        std::size_t size() const { return m_vector.size(); }
        std::size_t itemsNumber() const { return m_items; }
        bool unique() const { return m_unique; }
        void setUnique(bool u) { m_unique = u; }
        std::size_t maxIndex() const { return m_freeSlots + m_items; }
        bool contains(std::size_t i) const
        {
            return i < m_vector.size() && m_vector[i].has_value();
        }
        std::size_t indexOf(const VType v) const
        {
            for(int i = 0; i < m_vector.size(); i++)
                if (m_vector[i].has_value() && m_vector[i].value() == v)  return i;
            return InvalidIndex;
        }
        std::size_t add(VType v, std::size_t i = InvalidIndex)
        {
            if (m_unique && indexOf(v) != InvalidIndex)  return InvalidIndex;

            if (i != InvalidIndex) {
                std::size_t oldSize = m_vector.size();
                ensureSize(m_vector, i);
                if (m_vector[i].has_value()) {
                    m_vector[i] = v;
                    return i;
                }
                m_vector[i] = v;
                ++m_items;
                if (oldSize < i) // i is behind old array
                {
                    if (m_firstFreeSlot == InvalidIndex)
                        m_firstFreeSlot = oldSize;
                    m_freeSlots += m_vector.size() - oldSize - 1;
                }
                else { // i is inside array
                    if (m_firstFreeSlot == i)
                        updateFirstFreeSlot();
                    else {
                        --m_freeSlots;
                    }
                }

                return i;
            }
            if (m_firstFreeSlot != InvalidIndex)
            {
                auto i = m_firstFreeSlot;
                m_vector[i] = v;
                updateFirstFreeSlot();
                ++m_items;
                return i;
            }
            else
            {
                m_vector.push_back(v);
                ++m_items;
                return m_vector.size() - 1;
            }
            assert(("Should not be there!", false));
        }
        bool remove(std::size_t i)
        {
            if (contains(i))
            {
                if (i == m_vector.size() - 1)
                {
                    m_vector[i].reset();
                    m_vector.resize(i);
                }
                else
                {
                    m_vector[i].reset();
                    ++m_freeSlots;
                    if (m_firstFreeSlot == InvalidIndex || m_firstFreeSlot > i)
                        m_firstFreeSlot = i;
                }
                --m_items;
                return true;
            }
            return false;
        }
        const VType get(std::size_t i) const
        {
            return m_vector[i].value();
        }
        const VType get(std::size_t i, VType def) const
        {
            if (!isIndexValid(m_vector, i))  return def;
            return m_vector[i].value_or(def);
        }
        VType get(std::size_t i)
        {
            return m_vector[i].value();
        }
        VType get(std::size_t i, VType def)
        {
            if (!isIndexValid(m_vector, i))  return def;
            return m_vector[i].value_or(def);
        }
//         std::vector<std::optional<VType>>& getItems() { return m_vector; }
        const std::vector<std::optional<VType>>& getItems() const { return m_vector; }
        void clear()
        {
            m_vector.clear();
            m_firstFreeSlot = InvalidIndex;
            m_freeSlots = 0;
            m_items = 0;
        }
    };
