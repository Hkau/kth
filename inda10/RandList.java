import java.util.Random;

class RandList
{
	public static void main(String args[])
	{
		Random rand = new Random();
		int[] list = {1,2,3}; //new int[42];
		for(int i = 0; i < list.length; i++)
			list[i] = rand.nextInt(1000);
		System.out.print('[');
		for(int i = 0; i < list.length; i++)
		{
			System.out.print(list[i]);
			if(i != list.length - 1)
				System.out.print(", ");
		}
		System.out.println(']');
	}
};
