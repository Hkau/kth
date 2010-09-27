public class fib {
	public static void main(String args[]) {

		for(int n = 1; n <= 46; n++) {
			System.out.println("fib(" + n + ") = " + fib(n));
		}
	}

	public static int fib(int n) {
		if(n <= 2)
			return 1;
		return fib(n-1) + fib(n-2);
	}
}

