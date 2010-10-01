/**
 * A singly linked list.
 * 
 * @author slemming
 * @version 0x2A
 */
public class LinkedList<T> { 
    private ListElement<T> first;   // First element in list.
    private ListElement<T> last;    // Last element in list.
    private int size;               // Number of elements in list.
    
    /**
     * A list element.
     */
    private static class ListElement<T>
    {
        public T data;
        public ListElement<T> next;
        
        public ListElement(T data) {
            this.data = data;
            this.next = null;
        }
    }
    
    /**
     * Creates an empty list.
     */
    public LinkedList() {
    }

    /**
     * Inserts the given element at the beginning of this list.
     */
    public void addFirst(T element) {
        ListElement<T> foo = new ListElement<T>(element);
        foo.next = first;

        first = foo;
        size++;
        if(last == null)
            last = first;
    }

    /**
     * Inserts the given element at the end of this list.
     */
    public void addLast(T element) {
        ListElement<T> foo = new ListElement<T>(element);
        last.next = foo;

        last = foo;
        size++;
        if(first == null)
            first = last;
    }

    /**
     * Returns the first element of this list.
     * Returns <code>null</code> if the list is empty.
     */
    public T getFirst() {
        if(first == null)
            return null;
        return first.data;
    }

    /**
     * Returns the last element of this list.
     * Returns <code>null</code> if the list is empty.
     */
    public T getLast() {
        if(last == null)
            return null;
        return last.data;
    }

    /**
     * Returns the element at the specified position in this list.
     * Returns <code>null</code> if <code>index</code> is out of bounds.
     */
    public T get(int index) {
        ListElement<T> current = null;
        int i = 0;

        while(current != null)
        {
            if(i == index)
                return current.data;
            current = current.next;
            i++;
        }
        return null;
    }

    /**
     * Removes and returns the first element from this list.
     * Returns <code>null</code> if the list is empty.
     */
    public T removeFirst() {
        
        if(first == null)
            return null;

        if(last == first)
            last = null;

        T value = first.data;
        first = first.next;
        size--;

        return value;
    }

    /**
     * Removes all of the elements from this list.
     */
    public void clear() {
        first = null;
        last = null;
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

    /**
     * Returns a string representation of this list. The string
     * representation consists of a list of the elements enclosed in
     * square brackets ("[]"). Adjacent elements are separated by the
     * characters ", " (comma and space). Elements are converted to
     * strings by the method toString() inherited from Object.
     */
    public String toString() {
        String representation = "[";
        ListElement<T> current = first;

        if(current != null)
        {
            while(true)
            {
                representation += current.data.toString();

                current = current.next;

                if(current == null)
                    break;

                representation += ", ";
            }
        }
        representation += "]";

        return representation;
    }
 }

