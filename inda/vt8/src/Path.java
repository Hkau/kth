import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Stack;
import java.util.StringTokenizer;

import kth.csc.inda.*;

/**
 * @author lemming
 * Ännu ett fall för Karl Martin Frost!
 */
public class Path {

	/**
	 * @param args ignored
	 */
	public static void main(String[] args) {
		if(args.length != 2){
			System.out.println("Usage: java Path FROM TO");
			return;
		}
		int from, to;
		try{
			from = Integer.parseInt(args[0]);
			to = Integer.parseInt(args[1]);
		}
		catch(Exception e){
			System.out.println("Usage: java kth.csc.inda.Path FROM TO");
			return;
		}
		BufferedReader in;
		int nodes = 0;
		int numbers[] = new int[3]; // max 3 värden för en nod, från, till, vikt
		UndirectedGraph graph = new ListGraph(1); // will be replaced before usage
		try{
			in = new BufferedReader(new FileReader("Distances.txt"));
			String line;
			while((line = in.readLine())!= null){
				StringTokenizer st = new StringTokenizer(line);
				int size = 0;
				while(st.hasMoreTokens()){
					String token = st.nextToken();
					if(token.contains("//"))
						break;
					if(size == 3){
						System.err.println("Broken file");
						return;
					}

					numbers[size++] = Integer.parseInt(token);
				}
				if(size == 0)
					continue;
				if(nodes == 0){
					if(size != 1){
						System.err.println("Missing graph size");
						return;
					}
					nodes = numbers[0];
					
					if(Math.min(from, to) < 0 || Math.max(from, to) >= nodes){
						System.err.println("Start or end out of bounds");
						return;
					}
					graph = new ListGraph(nodes);
					continue;
				}
				if(size == 1){
					System.err.println("Multiple graph sizes or incomplete nodes");
					return;
				}
				if(size == 2) // Unweighed edge
					graph.addEdge(numbers[0], numbers[1]);

				graph.addEdge(numbers[0], numbers[1], numbers[2]);
			}
		}
		catch(FileNotFoundException e){
			System.err.println("File not found: \"Distances.txt\"");
			return;
		}
		catch(IOException e){
			System.err.println("IO error: " + e.getMessage());
			return;
		}
		catch(Exception e){
			System.err.println("Parse error: " + e.getMessage());
			return;
		}

		int visited[] = new int[nodes]; // which place the visitor came from, used for backtracing.

		for(int i = 0; i < nodes; ++i)	// init array with -1 as default, as 0 can a node which it origined from
			visited[i] = -1;

		// deque used for bfs
		Deque<Integer> deque = new ArrayDeque<Integer>(nodes);
		deque.add(from);
		
		// bfs
		Integer node;
		while((node = deque.poll()) != null){
			VertexIterator it = graph.adjacentVertices(node);
			while(it.hasNext()){
				int next = it.next();
				if(visited[next] != -1)
					continue;
				visited[next] = node;
				if(next == to) // framme! :D
					break;
				deque.add(next);
			}
			if(visited[to] != -1)
				break;
		}

		// if node wasn't reached
		if(visited[to] == -1){
			System.out.println(); //empty line
			return;
		}

		// search backwards to find way to get here
		Stack<Integer> stack = new Stack<Integer>();
		int pathNode = to;

		stack.push(to);
		do{
			pathNode = visited[pathNode];
			stack.push(pathNode);
		}while(pathNode != from);

		// print out path with fewest jumps

		while((pathNode = stack.pop()) != to)
			System.out.print("" + pathNode + " ");
		System.out.println(to);
	}
}
