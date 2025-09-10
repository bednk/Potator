#pragma once
#include <boost/pool/object_pool.hpp>

namespace Potator
{
	template <class T>
	class PooledObject
	{
		friend class boost::object_pool<T>;
	protected:
		inline static boost::object_pool<T> _pool;
	};
}