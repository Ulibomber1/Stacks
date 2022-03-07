template <typename C>
class Stack
{
public:

	typedef typename C::value_type value_type;

	Stack() {}

	int size() const { return theStack.size(); }

	bool empty() const { return theStack.empty(); }

	void push(const value_type& x)
	{
		theStack.push_back(x);
	}

	void pop() { theStack.pop_back(); }
	
	value_type top() { return theStack.back(); }

private:

	C theStack;
};