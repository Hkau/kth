/**
 * Postfix calculator. Used to evaluate postfix strings.
 * 
 * @author le ming
 * @version "0 FF x"
 */
public class PostFixCalculator
{	
	/**
	 * Evalutes a postfix expression.
	 * @param expression Postfix expression to be evaluated.
	 * @return Evaluated postfix expression as an int.
	 * @throws EmptyStackException Thrown if less than two operands are available upon operation.
	 * @throws NumberFormatException Occurs when something besides an int and valid operators are present.
	 * @throws IllegalArgumentException Occurs when there is more than one operand left after all calculations.
	 */
	public static int EvaluatePostFix(String expression)
	{
		Stack<Integer> stack = new MyStack<Integer>();
		if(expression.length() != 0) {
			for(String token : expression.split(" "))
			{
				if(isInt(token))
				{
					stack.push(Integer.valueOf(token));
					continue;
				}
	
				if(isOperator(token.charAt(0)))
				{
					char operator = token.charAt(0);
	
					int right = stack.pop(), left = stack.pop();
	
					int value = 0;
	
					switch(operator)
					{
						case '+':
							value=left+right;
							break;
						case '-':
							value=left-right;
							break;
						case '*':
							value=left*right;
							break;
						case '/':
							value=left/right;
							break;
						default:
							break;
					}
					stack.push(value);
	
					continue;
				}
				throw new NumberFormatException(); // Each token should be either an int or an operator.
			}
		}

		if(stack.size() != 1)
			throw new IllegalArgumentException(); // Still operands left, invalid expression.

		return stack.pop();
	}

	/**
	 * Determines a string's validity as an integer.
	 * @param s Input string.
	 * @return Returns true if the string is a valid int.
	 */
	private static boolean isInt(String s)
	{
		try{
			Integer.parseInt(s);
			return true;
		}
		catch (NumberFormatException e){
			return false;			
		}
	}

	/**
	 * Determines if a character is a valid operator.
	 * @param c Input character.
	 * @return Returns true if the character is a valid operator.
	 */
	private static boolean isOperator(char c)
	{
		return c == '+' || c == '-' || c == '*' || c == '/';
	}
}