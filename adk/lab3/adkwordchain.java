import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import java.util.List;
import java.util.LinkedList; // check performance vs. arraylist :)
import java.util.Arrays;

public class adkwordchain
{
	static class WordSet
	{
		short lastVisit;
		List<char[]> words;
		
		public WordSet()
		{
			words = new LinkedList<char[]>();
		}
	};

	private static WordSet words[][] = new WordSet[4][30*30*30]; // word sets

	final static private short stringCode(char s[], byte excl)
	{
		short code = 0;
		short mul = 1;
		for(byte i = 0; i < 4; ++i)
		{
			if(i == excl)
				continue;

			switch(s[i])
			{
				case 'å':
					code += 26*mul;
					break;
				case 'ä':
					code += 27*mul;
					break;
				case 'ö':
					code += 28*mul;
					break;
				case 'é':
					code += 29*mul;
					break;
				default:
					code += (s[i] - 'a') * mul;
			}
			mul *= 30;
		}
		return code;
	}

	private static short visit = 0;

	static class BFSnode
	{
		BFSnode parent;
		char value[];
		public BFSnode(char value[], BFSnode parent)
		{
			this.parent = parent;
			this.value = value;
		}

		public static void printChain(BFSnode node)
		{
			if(node.parent != null)
				printChain(node.parent);

			for(char c : node.value)
				System.out.print(c);

			System.out.print(" -> ");
		}
		public void printChain()
		{
			if(parent != null)
				printChain(parent);

			for(char c : value)
				System.out.print(c);
			System.out.println();
		}
		public int length()
		{
			if(parent == null)
				return 1;
			return 1 + parent.length();
		}
	};

	private static BFSnode bfs(char[] word)
	{
		++visit;
		LinkedList<BFSnode> bfsqueue = new LinkedList<BFSnode>();
		BFSnode node = new BFSnode(word, null);

		bfsqueue.addLast(node);

		while(!bfsqueue.isEmpty())
		{
			node = bfsqueue.removeFirst();

			for(byte i = 0; i < 4; ++i)
			{
				short codeIndex = stringCode(node.value, i);

				WordSet ws = words[i][codeIndex];

				if(ws == null || ws.lastVisit == visit)
					continue;

				ws.lastVisit = visit;

				for(char[] neigh : words[i][codeIndex].words)
					if(neigh[i] != node.value[i])
						bfsqueue.addLast(new BFSnode(neigh, node));
			}
		}

		return node;
	}

	private static BFSnode bfs(char[] from, char[] to)
	{
		++visit;
		LinkedList<BFSnode> bfsqueue = new LinkedList<BFSnode>();
		BFSnode node = new BFSnode(from, null);

		bfsqueue.addLast(node);

		while(!bfsqueue.isEmpty())
		{
			node = bfsqueue.removeFirst();

			for(byte i = 0; i < 4; ++i)
			{
				short codeIndex = stringCode(node.value, i);

				WordSet ws = words[i][codeIndex];

				if(ws == null || ws.lastVisit == visit)
					continue;

				ws.lastVisit = visit;

				for(char[] neigh : words[i][codeIndex].words)
				{
					if(Arrays.equals(neigh, to))
						return new BFSnode(neigh, node);
					bfsqueue.addLast(new BFSnode(neigh, node));
				}
			}
		}

		return null;
	}

	public static void main (String args[]) throws IOException
	{
		BufferedReader stdin =
			new BufferedReader(new InputStreamReader(System.in));
//		WordList.Read(WordLength, stdin);
//		LongestChain lc = new LongestChain(WordLength);

		while (true) {
			char word[] = new char[4];
			word[0] = (char)stdin.read();

			if(word[0] == '#')
				break;

			word[1] = (char)stdin.read();
			word[2] = (char)stdin.read();
			word[3] = (char)stdin.read();
			stdin.read();

			for(byte i = 0; i < 4; ++i)
			{
				short codeIndex = stringCode(word, i);

				if(words[i][codeIndex] == null)
				{
					WordSet ws = new WordSet();
					ws.lastVisit = 0;
					ws.words = new LinkedList<char[]>();
					ws.words.add(word);
					words[i][codeIndex] = ws;
				}
				else
					words[i][codeIndex].words.add(word);
			}
		}

		stdin.read();

		char from[] = new char[4];
		char to[] = new char[4];

		while (true) {
			String line = stdin.readLine();
			if (line == null) break;
			String tokens[] = line.split(" ");

			if (tokens.length == 1)
			{
				tokens[0].getChars(0, 4, from, 0);
				BFSnode node = bfs(from);
				for(char c :node.value)
					System.out.print(c);
				System.out.println(": " + node.length() + " ord");
				while(true)
				{
					//node.printChain();
					for(char c :node.value)
						System.out.print(c);
					if(node.parent == null)
						break;
					System.out.print(" -> ");
					node = node.parent;
				}
				System.out.println();

				continue;
			}
			else if (tokens.length == 2)
			{
				tokens[0].getChars(0, 4, from, 0);
				tokens[1].getChars(0, 4, to, 0);
				BFSnode node = bfs(from, to);
				if (node == null)
					System.out.println(tokens[0] + " " + 
							   tokens[1] + ": ingen lösning");
				else {
					System.out.println(tokens[0] + " " + 
							   tokens[1] + ": "+ node.length()
							   + " ord");
					node.printChain();
				}
			}
			else
			{
				// behövs?
				System.out.println("felaktig fråga: '" + line + "'");
				System.out.println("syntax för frågor: slutord");
				System.out.println("eller:			 startord slutord");
			}
		}
	}
}
