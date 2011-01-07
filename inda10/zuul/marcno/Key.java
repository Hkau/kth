/*
 * Key class, holds keys to doors.
 */
public class Key {
	
	String keyName;
	
	public Key(String keyName)
	{
		this.keyName = keyName;
	}

   /**
	* return name of key
	*/	
	String getName()
	{
		return keyName;
	}
}
