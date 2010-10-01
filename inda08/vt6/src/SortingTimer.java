import java.util.Random;

/**
 * @author lemming
 * Timer class for different sorting algorithms, program induces JIT,
 * performs different test cases and print out the results and how
 * they compare to the Java implementation Arrays.sort(v);
 */
public class SortingTimer {
	// Antalet algoritmer som finns att testa med
	private static final int algs = 5;

	// Sorteringsalgoritmer
	private static Sorting sorters[] = {
		new QuicksortJava(),
		new Quicksort(),
		new QuicksortRandomPivot(),
		new QuicksortInsertion(),
		new QuicksortInsertionRandom()/*,
		new InsertionSort()*/};

	// Namn för algoritmerna, för att identifiera tidtagningarna 
	private static String sortingName[] =
		{"Java Quicksort implementation",
		"Quicksort",
		"Quicksort with random pivot",
		"Quicksort with insertion sort",
		"Quicksort with insertion sort and random pivot"/*,
		"Insertion sort"()*/};

	// Listor för talen som ska sorteras, en lista per algoritm
	private static int numbers[][] = new int[algs][];

	// Slumpgenerator
	private static Random rand = new Random();

	/**
	 * Kopierar första listan till resten av listorna.
	 */
	private static void copyArrays(){
		for(int i = 1; i < algs; i++){
			numbers[i] = new int[numbers[0].length];
			System.arraycopy(numbers[0], 0, numbers[i], 0, numbers[0].length);
		}
	}

	/**
	 * Sorteringsprogrammet, skapar olika listtyper och ser till att de testas.
	 * @param args Unused
	 */
	public static void main(String[] args){

		// Kör alla sorteringsalgoritmer på stora slumpade listor för att tvinga fram JIT
		System.out.println("Inducing JIT compilation through repeatedly calling sort on random elements:");
		
		for(int i = 0; i < 5; ++i){

			numbers[0] = new int[1000000];

			for(int j = 0; j < 1000000; ++j){
				numbers[0][j] = rand.nextInt();
			}

			copyArrays();
			sort();
		}

		// JIT klar, starta testning
		System.out.println("\nStarting test!\n");

		// Rättsorterad lista [0, 1, ..., n-1]
		System.out.println("\nSorted list, 1000000 elements:\n");
		numbers[0] = new int[1000000];

		for(int j = 0; j < 1000000; ++j){
			numbers[0][j] = j;
		}

		copyArrays();
		sort();

		// Felsorterad lista [n, n-1, ..., 1]
		System.out.println("\nReversely sorted list, 1000000 elements:\n");
		numbers[0] = new int[1000000];

		for(int j = 0; j < 1000000; ++j){
			numbers[0][j] = 1000000-j;
		}

		copyArrays();
		sort();

		// Blandad lista som ser ut som två ihopslagna listor.
		// v[0, 2...] ökar medan v[1, 3...] minskar
		System.out.println("\nMixed sorted list, 1000000 elements:\n");
		numbers[0] = new int[1000000];

		for(int j = 0; j < 1000000; ++j){
			if((j & 1) == 0) numbers[0][j] = j;
			else numbers[0][j] = 1000000 - j;
		}

		copyArrays();
		sort();

		// Lista med ekvivalenta element
		System.out.println("\nSame value, 1000000 elements:\n");
		numbers[0] = new int[1000000];

		for(int j = 0; j < 1000000; ++j){
			numbers[0][j] = 42;
		}
	
		copyArrays();
		sort();

		// Lista med två olika värden
		System.out.println("\nEttnoll, 1000000 elements:\n");
		numbers[0] = new int[1000000];

		for(int j = 0; j < 1000000; ++j){
			numbers[0][j] = rand.nextInt(2);
		}

		copyArrays();
		sort();

		//Lista med 10 olika värden [0, 1, ... 9]
		System.out.println("\nMOD 10, 1000000 elements:\n");
		numbers[0] = new int[1000000];

		for(int j = 0; j < 1000000; ++j){
			numbers[0][j] = rand.nextInt(10);
		}

		copyArrays();
		sort();

		// Lista med 1000 olika värden
		System.out.println("\nMOD 1000, 1000000 elements:\n");
		numbers[0] = new int[1000000];

		for(int j = 0; j < 1000000; ++j){
			numbers[0][j] = rand.nextInt(1000);
		}

		copyArrays();
		sort();

		// Slumpad lista, 1 000 element
		System.out.println("\nRandom list, 1 000 elements:\n");
		numbers[0] = new int[1000];

		for(int j = 0; j < 1000; ++j){
			numbers[0][j] = rand.nextInt();
		}

		copyArrays();
		sort();

		// Slumpad lista 10 000 element
		System.out.println("\nRandom list, 10 000 elements:\n");
		numbers[0] = new int[10000];

		for(int j = 0; j < 10000; ++j){
			numbers[0][j] = rand.nextInt();
		}

		copyArrays();
		sort();

		// Slumpad lista 100 000 element
		System.out.println("\nRandom list, 100 000 elements:\n");
		numbers[0] = new int[100000];

		for(int j = 0; j < 100000; ++j){
			numbers[0][j] = rand.nextInt();
		}

		copyArrays();
		sort();

		// Slumpad lista 1 000 000 element
		System.out.println("\nRandom list, 1 000 000 elements:\n");
		numbers[0] = new int[1000000];
	
		for(int j = 0; j < 1000000; ++j){
			numbers[0][j] = rand.nextInt();
		}
		
		copyArrays();
		sort();
	}

	/**
	 * Sorterar varje med varje algoritm, skriver ut tid och hur de jämför sig
	 * med javas Arrays.sort() (algoritm #0)
	 */
	private static void sort(){
		Stopwatch s = new Stopwatch();

		s.reset();
		s.start();
		sorters[0].sort(numbers[0]);	
		s.stop();
		System.out.println(sortingName[0] + ": " + s.toString());

		long time = s.getTime();
		
		for(int j = 1; j < algs; ++j){
			s.reset();
			s.start();
			sorters[j].sort(numbers[j]);	
			s.stop();
			String ratio = (time == 0) ? "?" : Double.toString((((double)s.getTime())/time));
			System.out.println(sortingName[j] + ": " + s.toString() + " - " + ratio);
		}
	}
}
