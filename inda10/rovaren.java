public class rovaren {
	public static void main(String args[]) {
		System.out.println(toRovarspraket("laser"));
	}
	public static String toRovarspraket(String str) {
		String out = new String();
		for(int i = 0; i < str.length(); ++i) {
			char c = str.charAt(i);

			if(isConsonant(c)) {
				out += c;
				out += 'o';
				out += c;
			}
			else
				out += c;
		}
		return out;
	}
	public static boolean isConsonant(char c) {
		String consonants = "bcdfghjklmnpqrstvwxz";
		for(int i = 0; i < consonants.length(); ++i) {
			char cons = consonants.charAt(i);
			if(c == cons)
				return true;
		}
		return false;
	}
}

