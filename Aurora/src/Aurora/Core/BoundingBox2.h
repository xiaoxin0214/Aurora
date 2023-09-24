#pragma once
#include "Core.h"
namespace Aurora
{
	template<typename T = float>
	class AURORA_API BoundingBox2
	{
	public:
		BoundingBox2() :xmin(std::numeric_limits<T>::max()), ymin(std::numeric_limits<T>::max()),
			xmax(std::numeric_limits<T>::lowest()), ymax(std::numeric_limits<T>::lowest())
		{

		}

		BoundingBox2(T xminIn, T yminIn, T xmaxIn, T ymaxIn) :xmin(xminIn), ymin(yminIn), xmax(xmaxIn), ymax(ymaxIn)
		{

		}

	public:
		void Set(T xminIn,T yminIn, T xmaxIn, T ymaxIn)
		{
			xmin = xminIn;
			ymin = yminIn;
			xmax = xmaxIn;
			ymax = ymaxIn;
		}

		bool IsValid()
		{
			if (xmax < xmin || ymax < ymin)
				return false;

			return true;
		}

		bool Contains(T x, T y)
		{
			if (x <= xmin || x >= xmax || y <= ymin || y >= ymax)
				return false;

			return true;
		}

		T Width()
		{
			return xmax - xmin;
		}

		T Height()
		{
			return ymax - ymin;
		}
	public:
		T xmin;
		T ymin;
		T xmax;
		T ymax;
	};
}