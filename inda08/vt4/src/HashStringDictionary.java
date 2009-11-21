import java.util.List;
import java.util.LinkedList;

/**
 * @author	lemming
 * @version	0xFF
 */
public class HashStringDictionary implements StringDictionary{
    private List<String>[] table;
    private int lists;

    /**
     * Creates a hash table with the given capacity.
     * 
     * @throws IllegalArgumentException if capacity <= 0.
     */
    public HashStringDictionary(int capacity) {
        if (capacity <= 0)
            throw new IllegalArgumentException("capacity <= 0");

        /*
         * The array will contain only LinkedList<String>
         * instances, all created in the add method. This
         * is sufficient to ensure type safety.
         */
        @SuppressWarnings("unchecked")
            List<String>[] t = new LinkedList[capacity];
        table = t;

    	for(int i = 0; i < capacity; ++i){
    		table[i] = new LinkedList<String>();
    	}

        lists = capacity;
    }

    /**
     * Adds the given string to this dictionary.
     * Returns <code>true</code> if the dictionary
     * did not already contain the given string.
     *
     * Complexity: O(1) expected time.
     */
    public boolean add(String s) {
    	if(contains(s))
    		return false;

    	table[Math.abs(s.hashCode() % lists)].add(s);
    	return true;
    }

    /**
     * Removes the given string from this dictionary
     * if it is present. Returns <code>true</code> if
     * the dictionary contained the specified element.
     *
     * Complexity: O(1) expected time.
     */
    public boolean remove(String s) {
        return table[Math.abs(s.hashCode() % lists)].remove(s);
    }

    /**
     * Returns <code>true</code> if the string is
     * in this dictionary.
     *
     * Complexity: O(1) expected time.
     */
    public boolean contains(String s) {
        return table[Math.abs(s.hashCode() % lists)].contains(s);
    }
}
