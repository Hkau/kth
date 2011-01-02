class mariemax
{
	public static void main(String[] args)
	{
		int[] v = {4,5,3,1};
		System.out.println("" + max(v));
	}
	public static int max(int[] v)
	{
		int returnValue = helpMethod(v, 0);
		return v[returnValue];
	}
	private static int helpMethod(int[] vector, int current)
	{
		if (current == vector.length-1)
			return current;

		int index = helpMethod(vector, current+1);
		if (vector[current] > vector[index]){
			index=current;
		}
		return index;
	}
}

