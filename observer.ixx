export module observer;

import <vector>;
import <algorithm>;

// observer design pattern
export template<class Observable> struct observer
{
	virtual void update(Observable&) = 0;
};

export class observable
{
public:
	inline void attach(observer<observable>& o)
	{
		m_observers.push_back(&o);
	}
	inline void detach(observer<observable>& o)
	{
		m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), &o));
	}
	inline void notify()
	{
		for (auto* o : m_observers)
			o->update(*this);
	}

private:
	std::vector<observer<observable>*> m_observers;
};
