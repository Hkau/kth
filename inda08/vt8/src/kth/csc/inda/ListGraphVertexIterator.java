/**
 * 
 */
package kth.csc.inda;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * @author lemming
 *
 */
public class ListGraphVertexIterator implements VertexIterator{
	private Iterator<Integer> iter;
	public ListGraphVertexIterator(Iterator<Integer> it){
		iter = it;
	}
	public boolean hasNext(){
		return iter.hasNext();
	}
	public int next() throws NoSuchElementException{
		return iter.next().intValue();
	}
}
