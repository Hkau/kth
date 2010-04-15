/**
 * @author lemming
 * Quicksortvariant som byter till insertion sort för tillräckligt små listor
 */
public class QuicksortInsertion extends Quicksort{
	/**
	 * Quicksort av lista som växlar till insertionsort för mindre listor 
	 */
	public void qsort(int[] v, int first, int last){
		if(first >= last)
			return;

		// Växla till insertion sort, enda skillnaden i quicksort-varianten
		if(last-first <= 16){
			insertionSort(v, first, last);
			return;
		}

		// Annars fortsätt som vanligt
		int mid = partition(v, first, last, getPivot(v, first, last));

		qsort(v, first, mid);
		qsort(v, mid+1, last);
	}

	private void insertionSort(int[] v, int first, int last){
		for(int i = first+1; i < last+1; ++i){
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
