/**
 * Person class, implements Comparable to be used with TreeSet.
 */

public class Person implements Comparable<Person>
{
	private int age;

	/**
	 * Person constructor.
	 * @param age Person's age.
	 */
	public Person(int age) {
		this.age = age;
	}

	/**
	 * Compares the Person's age to another's.
	 * @param p Other operand.
	 * @return Difference between this person's age and another's.
	 */
	public int compareTo(Person p){
		return age - p.getAge();
	}

	/**
	 * Get age.
	 * @return The person's age.
	 */
	public int getAge() {
		return age;
	}
}
