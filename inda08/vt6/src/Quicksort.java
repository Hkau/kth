/**
 * @author lemming
 * Egen implementation av Quicksort
 */
public class Quicksort implements Sorting{
	/**
	 * Sortera en array
	 * @param v array att sortera
	 */
	public void sort(int[] v){
		qsort(v, 0, v.length-1);
	}

	/**
	 * Sorterar alla element mellan first, last i en array med quicksort.
	 * @param v	array
	 * @param first	första elementet
	 * @param last	sista elementet
	 */
	public void qsort(int[] v, int first, int last){
		// Listan innehåller färre än 2 element och behöver inte sorteras
		if(first >= last)
			return;

		// sortera upp listan i två delar, en som är större än eller lika med pivot, och en mindre 
		int mid = partition(v, first, last, getPivot(v, first, last));

		//Sortera dessa mindre delar
		qsort(v, first, mid);
		qsort(v, mid+1, last);
	}

	/**
	 * Sorterar en del av en lista.
	 * @param v Lista att sortera i
	 * @param first Första elementet att sortera
	 * @param last Sista elementet i listan som ska sorteras
	 * @param pivot Pivot som används vid sortering
	 * @return index till mitten av listan
	 */
	public int partition(int[] v, int first, int last, int pivot){
		// använder sig av två index som löper mot mitten
		int low = first, high = last;

		//Så länge hela listan inte är genomgådd
		while(low<=high){
			// Hitta första elementet som är större än eller lika med pivot
			while(v[low] < pivot)
				low++;
			// Hitta sista elementet som är mindre än eller lika med pivot 
			while(v[high] > pivot)
				high--;
			// Avbryt om alla element redan är hittade
			if(low>high)
				break;
			
			//Dessa två element ska byta plats
			int tmp = v[low];
			v[low] = v[high];
			v[high] = tmp;
			low++;
			high--;
		}
		// Returnera index till mitten
		return low-1;
	}

	protected int getPivot(int[] v, int first, int last){
		return v[first + (last-first)/2];
	}
}