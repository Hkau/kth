/**
 * Stack interface
 * @author lemming
 * @version shoop'
 */
public interface Stack<T> {
	public void push(T o);
	public T pop();
	public T top();
	public int size();
	public boolean isEmpty();
}
