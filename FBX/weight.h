#pragma once

class Weight
{
public:

	Weight() 
		:Weight(0, 0)
	{}
	
	Weight(const int& i, const float& w) 
		:mIndex(i), mWeight(w) 
	{}

	const int&		GetIndex() const;
	void			SetIndex(const int &i);

	const float&	GetWeight() const;
	void			SetWeight(const float &w);
private:
	int mIndex;
	float mWeight;
};

inline const int& Weight::GetIndex() const
{
	return mIndex;
}

inline void	Weight::SetIndex(const int &i)
{
	mIndex = i;
}

inline const float&	Weight::GetWeight() const
{
	return mWeight;
}

inline void	Weight::SetWeight(const float &w)
{
	mWeight = w;
}