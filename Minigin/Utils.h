#pragma once
namespace dae 
{
	template<class T, class U> struct Pair
	{
		Pair(T cfirst, U csecond)
			: first{ cfirst }
			, second{ csecond }
		{}			

		T first;
		U second;
	};
}