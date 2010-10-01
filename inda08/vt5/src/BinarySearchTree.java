/**
 * BINÄRSÖKTRÄDSMONSTERMONSTER
 * @author lemming
 */
public class BinarySearchTree<T extends Comparable<? super T>> {
	private T value;
	private BinarySearchTree<T> less, more;

	/**
	 * Constructs a bst with specified value for root node. 
	 * @param value Value for the (root) node.
	 */
	public BinarySearchTree(T value){
		this.value = value;
	}

	/**
	 * Add an object to the tree.
	 * @param object Object to be added.
	 * @return Returns true if the object was added, or false if it already existed.
	 */
	public boolean add(T object){
		BinarySearchTree<T> currentNode = this;

		while(true){
			int diff = object.compareTo(currentNode.value);
			if(diff == 0)
				return false;
			if(diff < 0){
				if(currentNode.less != null){
					currentNode = currentNode.less;
					continue;
				}
				currentNode.less = new BinarySearchTree<T>(object);
				return true;
			}
			if(currentNode.more != null){
				currentNode = currentNode.more;
				continue;
			}
			currentNode.more = new BinarySearchTree<T>(object);
			return true;
		}
	}

	/**
	 * Check for object is contained in tree.
	 * @param object Value to be looked for
	 * @return Returns true if the object was found inside the tree.
	 */
	public boolean contains(T object){
		BinarySearchTree<T> currentNode = this;

		while(true){
			int diff = object.compareTo(currentNode.value);
			if(diff == 0)
				return true;
			if(diff < 0){
				if(currentNode.less != null){
					currentNode = currentNode.less;
					continue;
				}
			}
			else if(currentNode.more != null){
				currentNode = currentNode.more;
				continue;
			}

			return false;
		}
	}

	/**
	 * Get size of tree.
	 * @return Number of elements in tree.
	 */
	public int size(){
		if(less == null && more == null)
			return 1;
		if(less == null)
			return more.size()+1;
		if(more == null)
			return less.size()+1;
		return less.size() + more.size() + 1;
	}

	/**
	 * Calculates tree height.
	 * @return How tall the tree is.
	 */
	public int height(){
		return internal_height(this);
	}

	/**
	 * Internally used to calculate tree height.
	 * @param current Node to calculate from.
	 * @return height from current node.
	 */
	private int internal_height(BinarySearchTree<T> current){
		if(current == null)
			return -1;
		return 1 + Math.max(internal_height(current.less), internal_height(current.more));
	}

	/**
	 * Counts amount of leaves in tree.
	 * @return Amount of leaves.
	 */
	public int leaves(){
		if(less == null && more == null)
			return 1;
		if(less == null)
			return more.leaves();
		if(more == null)
			return less.leaves();
		return less.leaves() + more.leaves();
	}

	/**
	 * Returns string representation of tree.
	 * @return The tree as a string.
	 */
	public String toString(){
		String string = new String();
		if(less != null)
			string += less.toString() + ' ';
		string += value.toString() + ' ';
		if(more != null)
			string += more.toString();
		return string.trim();
	}
}
