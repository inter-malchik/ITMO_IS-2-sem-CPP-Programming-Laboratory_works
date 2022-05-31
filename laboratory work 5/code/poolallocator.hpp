#ifndef CUSTOMPOOLALLOCATOR_POOLALLOCATOR_HPP
#define CUSTOMPOOLALLOCATOR_POOLALLOCATOR_HPP

#include <iostream>
#include <exception>
#include <cstdlib>
#include <vector>
#include <memory>

#define CHUNKS_BY_DEFAULT 100l
#define SIZE_BY_DEFAULT 4l
#define LOGGING false

namespace customalloc {
    template<typename T>
    class PoolAllocator {
    public:
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using pointer = T *;
        using const_pointer = const T *;
        using reference = T &;
        using const_reference = const T &;
        using value_type = T;


        explicit PoolAllocator(size_t ChunkAmount, size_t ChunkSize = 1) : ChunkAmount(ChunkAmount),
                                                                           ChunkSize(ChunkSize),
                                                                           ChunkSizeBytes(ChunkSize * TSize),
                                                                           chunks(std::vector<MemoryChunk>(
                                                                                   ChunkAmount)) {
            start_point = init_point = static_cast<pointer>(calloc(ChunkAmount, ChunkSize * TSize));
            // chunks = static_cast<MemoryChunk*>(calloc(ChunkAmount, sizeof(MemoryChunk)));
            if (init_point == nullptr /*|| chunks == nullptr*/)
                throw std::bad_alloc();

            for (size_t i = 0; i < ChunkAmount; i++) {
                if (!i)
                    chunks[0].p = init_point;
                else
                    chunks[i].p = chunks[i - 1].p + ChunkSize;
                chunks[i].sized = ChunkSize * TSize;
            }
            absorb_chunks();
        }

        PoolAllocator() : PoolAllocator(CHUNKS_BY_DEFAULT, SIZE_BY_DEFAULT) {}

        PoolAllocator(const PoolAllocator &other) : PoolAllocator(other.ChunkAmount, other.ChunkSize) {
            for (size_t i = 0; i < ChunkAmount; i++) {
                chunks[i].used = other.chunks[i].used;
                chunks[i].sized = other.chunks[i].sized;
                memcpy(chunks[i].p, other.chunks[i].p, chunks[i].sized);
            }
        }

        PoolAllocator operator=(const PoolAllocator &other) = delete;


        pointer allocate(size_type n) {
            if (LOGGING)
                std::cout << "A request to allocate " << n << " objects" << std::endl;
            n *= TSize;

            if (n % (ChunkSizeBytes))
                n += ChunkSizeBytes - n % (ChunkSizeBytes);
            size_t cur_ind = get_index_by_pointer(start_point);
            for (; cur_ind < ChunkAmount; cur_ind++) {
                if (chunks[cur_ind].used) continue;
                size_t chunkers = get_amount_of_chunks_by_bytesize(chunks[cur_ind].sized);
                if (chunks[cur_ind].sized >= n) {
                    if (chunkers >
                        get_amount_of_chunks_by_bytesize(n)) {
                        /*size_t ender = cur_ind + get_amount_of_chunks_by_bytesize(chunks[cur_ind].sized);

                        for (size_t i = cur_ind + get_amount_of_chunks_by_bytesize(n);
                             i < ender; i++)
                            chunks[i] = {false, ChunkSize * TSize, chunks[i].p};
                         */
                        chunks[cur_ind + get_amount_of_chunks_by_bytesize(n)].sized = chunks[cur_ind].sized - n;
                        chunks[cur_ind + get_amount_of_chunks_by_bytesize(n)].used = false;

                        chunks[cur_ind].sized -=
                                (get_amount_of_chunks_by_bytesize(chunks[cur_ind].sized) -
                                 get_amount_of_chunks_by_bytesize(n)) *
                                ChunkSize * TSize;
                    }
                    if (LOGGING)
                        std::cout << "Allocated " << n << " bytes at " << chunks[cur_ind].p << '\n' << std::endl;
                    chunks[cur_ind].used = true;
                    return chunks[cur_ind].p;
                }
            }
            throw std::bad_alloc();
        }

        void deallocate(pointer p, size_type n) {
            if (p < init_point || p > init_point + ChunkSizeBytes * ChunkAmount)
                return;
            if (LOGGING)
                std::cout << "A request to deallocate " << n << " objects" << std::endl;
            n *= TSize;
            if (n % (ChunkSizeBytes))
                n += ChunkSizeBytes - n % (ChunkSizeBytes);
            size_type s = get_index_by_pointer(p);
            while (true) {
                chunks[s].used = false;
                if (chunks[s].sized >= n)
                    break;
                else
                    n -= chunks[s].sized;
                s++;
            }
            if (LOGGING)
                std::cout << "Deallocated " << n << " bytes from " << p << '\n' << std::endl;
            absorb_chunks();
        }

        void free() {
            for (const MemoryChunk &ch: chunks)
                ch.used = false;
            absorb_chunks();
        }

        ~PoolAllocator() {
            std::free(init_point);
        }

    private:
        struct MemoryChunk {
            bool used = false;
            size_t sized = TSize;
            pointer p = nullptr;
        };

        pointer init_point;
        pointer start_point;
        static const size_t TSize = sizeof(value_type);
        const size_t ChunkSizeBytes;
        size_type ChunkAmount;
        size_type ChunkSize;
        std::vector<MemoryChunk> chunks;

        constexpr difference_type get_index_by_pointer(pointer &p) {
            return (p - init_point) / ChunkSize;
        }

        constexpr size_t get_amount_of_chunks_by_bytesize(size_t size) {
            return size / ChunkSizeBytes;
        }

        void absorb_chunks() {
            for (size_t i = 0; i < ChunkAmount; i++)
                for (size_t j = 1; i + j < ChunkAmount; j += chunks[i + j].sized / ChunkSizeBytes) {
                    if (!chunks[i].used && !chunks[i + j].used) {
                        chunks[i].sized += chunks[i + j].sized;
                        chunks[i + j].used = true;
                    } else {
                        i += j;
                        break;
                    }
                }

        }

    };
}

#endif //CUSTOMPOOLALLOCATOR_POOLALLOCATOR_HPP
