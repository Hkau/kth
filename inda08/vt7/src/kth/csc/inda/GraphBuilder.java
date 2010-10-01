/**
 * 
 */
package kth.csc.inda;

import java.util.Random;

/**
 * @author lemming
 * Denna jäkeln bygger grafer åt den som händer runt 0 grader.
 */
public class GraphBuilder {
	private static Random rand = new Random();

	/**
	 * @param args ignored
	 */
	public static void main(String[] args) {
		final int n = 100000;
		ListGraph lg = generateRandomGraph(n);
		
		boolean [] visited = new boolean[n];

		int maxNodes = 0;
		int clusters = 0;

		long ms = System.currentTimeMillis();

		for(int i = 0; i < n; ++i)
		{
			if(visited[i] == true)
				continue;
			int nodes = DFS(lg, i, visited);
			if(maxNodes < nodes)
				maxNodes = nodes;
			clusters++;
		}
		ms = System.currentTimeMillis() - ms;
		System.out.println("Vertices/edges: " + n);
		System.out.println("Clusters: " + clusters);
		System.out.println("Largest cluster: " + maxNodes);
		System.out.println("Cluster analysis: " + ms + " ms");
	}

	static private int DFS(UndirectedGraph g, int start, boolean[] visited)
	{
		int visitedNodes = 1;
		for(VertexIterator it = g.adjacentVertices(start); it.hasNext();){
			int i = it.next();
			if(visited[i])
				continue;
			visited[i] = true;
			visitedNodes += DFS(g, i, visited);
		}
		return visitedNodes;
	}
	
	static private ListGraph generateRandomGraph(int size){ // Uppgift specifierar n noder och n kanter

		ListGraph lg = new ListGraph(size);
		int left = size;

		// Eftersom size antas vara ganska stor är size mycket mindre än size^2 vilket gör 
		// att vi kan pröva oss fram till om en kant passar eller inte.
		do{
			int x = rand.nextInt(size), y = rand.nextInt(size);
			if(lg.areAdjacent(x, y)) //Already existed.
				continue;
			lg.addEdge(x, y);
			left--;
		}while(left > 0);
		
		return lg;
	}

}
