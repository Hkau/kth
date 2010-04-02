/**
 * A singly linked stack.
 * 
 * @author slemming
 * @version 0x2A? OR IS IT!?
 */
public class MyStack<T> implements Stack<T> { 
    private StackElement<T> first;   // Top element in stack.
    private int size;               // Number of elements in stack.
    
    /**
     * A stack element.
     */
    private static class StackElement<T>
    {
        public T data;
        public StackElement<T> next;

        public StackElement(T data) {
            this.data = data;
            this.next = null;
        }
    }
    
    /**
     * Creates an empty stack.
     */
    public MyStack() {
    }

    /**
     * Inserts the given element at the top of the stack.
     */
    public void push(T element) {
        StackElement<T> foo = new StackElement<T>(element);
        foo.next = first;

        first = foo;
        size++;
    }

    /**
     * Removes and returns the first element on this stack.
     */
    public T pop() {
        if(first == null)
            throw new java.util.EmptyStackException();

        T value = first.data;
        first = first.next;
        size--;

        return value;
    }

    /**
     * Gives top element of the stack without removing it.
     */
    public T top() {
    	if(first == null)
    		throw new java.util.EmptyStackException();
    	return first.data;
    }

    /**
     * Removes all of the elements from this stack.
     */
    public void clear() {
        first = null;
        size = 0;
    }

    /**
     * Returns the number of elements in this list.
     */
    public int size() {
        return size;
    }

    /**
     * Returns <code>true</code> if this list contains no elements.
     */
    public boolean isEmpty() {
        return size == 0;
    }
 }
