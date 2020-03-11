#pragma once

#include "utility/Exceptions.h"

#include <utility>
#include <memory>
#include <vector>

namespace simple_math
{
	/// \brief A block vector data structure.
	///
	/// At construction, the block vector consists of null blocks until a block is set.
	/// The null blocks can be used to indicate zero blocks efficiently.
	template<typename VectorType>
	class BlockVector
	{
	public:
		BlockVector(const unsigned int n_blocks);
		BlockVector(BlockVector&& other);

		/// Set the i-th block by copy assignment
		void set_block(const unsigned int i, const VectorType & v);
		/// Set the i-th block by moving v_ptr to this structure
		void set_block(const unsigned int i, std::unique_ptr<VectorType> v_ptr);
		/// Set the i-th block of this block vector by moving the j-th block of block_v
		/// \param i - the block index of this structure to set
		/// \param block_v - a r-value block vector to take a block from
		/// \param j - the block index of block_v to take the block from
		void set_block(const unsigned int i, BlockVector&& block_v, const unsigned int j);

        /// Add the given vector the indicated block.  Sets the block if it is null.
        void add_to_block(const unsigned int i, const VectorType& v);
        /// Adds j-th block of block_v to the i-th block of this block matrix
        /// \param i - the row of this structure to set
        /// \param block_v - a r-value block vector to take a block from
        /// \param j - the row of block_v to take the block from
        void add_to_block(const unsigned int i,
            const BlockVector& block_v,
            const unsigned int j);

		/// If i-th is a nullptr, this will initialize it to an empty vector
		void init_empty_block(const unsigned int i);
		/// Returns a bool indicating if the i-th block is a nullptr.
		/// This is be used before a call to BlockVector::get_block.
		bool is_block_null(const unsigned int i) const;
		/// Access the i-th block by const-reference
		/// \attention This will throw if the ith block is null
		VectorType& get_block(const unsigned int i);
        const VectorType& get_block(const unsigned int i) const;
		/// Get the number of blocks
		unsigned int get_n_blocks() const;

	private:
		std::vector<std::unique_ptr<VectorType>> blocks_;
	};

	template<typename VectorType>
	inline BlockVector<VectorType>::BlockVector(const unsigned int n_blocks)
	{
		blocks_.resize(n_blocks);
	}

	template<typename VectorType>
	inline BlockVector<VectorType>::BlockVector(BlockVector && other)
	{
		blocks_.swap(other.blocks_);
	}

	template<typename VectorType>
	inline void BlockVector<VectorType>::set_block(const unsigned int i, const VectorType & v)
	{
		if (is_block_null(i))
		{
			blocks_[i] = std::make_unique<VectorType>();
		}
		*blocks_[i] = v;
	}

	template<typename VectorType>
	inline void BlockVector<VectorType>::set_block(const unsigned int i, std::unique_ptr<VectorType> v_ptr)
	{
		blocks_[i] = std::move(v_ptr);
	}

	template<typename VectorType>
	inline void BlockVector<VectorType>::set_block(const unsigned int i, BlockVector && block_v, unsigned int j)
	{
		blocks_[i] = std::move(block_v.blocks_[j]);
	}

    template<typename VectorType>
    inline void BlockVector<VectorType>::add_to_block(const unsigned int i, const VectorType& v)
    {
        if (is_block_null(i))
        {
            blocks_[i] = std::make_unique<VectorType>(v);
        }
        else
        {
            *blocks_[i] += v;
        }
    }

    template<typename VectorType>
    inline void BlockVector<VectorType>::add_to_block(const unsigned int i, const BlockVector& block_v, const unsigned int j)
    {
        if (!block_v.is_block_null(j))
        {
            add_to_block(i, block_v.get_block(j));
        }
    }

    template<typename VectorType>
	inline void BlockVector<VectorType>::init_empty_block(const unsigned int i)
	{
		if (is_block_null(i))
		{
			blocks_[i] = std::make_unique<VectorType>();
		}
	}

	template<typename VectorType>
	inline bool BlockVector<VectorType>::is_block_null(const unsigned int i) const
	{
		return blocks_[i] == nullptr;
	}

	template<typename VectorType>
	inline VectorType & BlockVector<VectorType>::get_block(const unsigned int i)
	{
		return *blocks_[i];
	}

    template<typename VectorType>
    inline const VectorType& BlockVector<VectorType>::get_block(const unsigned int i) const
    {
        return *blocks_[i];
    }

	template<typename VectorType>
	inline unsigned int BlockVector<VectorType>::get_n_blocks() const
	{
		return static_cast<unsigned int>(blocks_.size());
	}
};
