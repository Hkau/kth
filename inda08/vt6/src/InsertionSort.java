/**
 * @author lemming
 * Implementation av Insertion sort.
 * 
 * Sorterar monster (Se Pokédex för referens av vilka monster som sorteras).
 */
public class InsertionSort implements Sorting{

	/**
	 * Sortera en array av intar
	 * @param v array att sortera
	 */
	public void sort(int[] v){
		for(int i = 1; i < v.length; ++i){
			if(v[i-1]>v[i]){
				int tmp = v[i];

				int j = i;

				while(j > 0 && v[j-1] > tmp){
					v[j] = v[--j];
				}
				v[j] = tmp;
			}
		}
	}
}
