/**
 * Remodelling of the previously naive TicketMachine
 *
 * @author Peter Boström, originally David J. Barnes and Michael Kolling
 * @version 2
 */
public class TicketMachine
{
    // The price of a ticket from this machine.
    private int price;
    // The amount of money entered by a customer so far.
    private int balance;
    // The total amount of money collected by this machine.
    private int total;

    /**
     * Create a machine that issues tickets of the given price.
     * Note that the price must be greater than zero, and there
     * are no checks to ensure this.
     * @param newPrice The price of tickets.
     */

    public TicketMachine(int newPrice)
    {
        price = newPrice;
        balance = 0;
        total = 0;
    }
    
    /**
     * Create a TicketMachine with a default price of 1000 cents.
     */

    public TicketMachine()
    {
        price = 1000;
        balance = 0;
        total = 0;
    }
    
    /**
     * Empties out the TicketMachine. Sets total and balance to zero.
     */

    public void empty()
    {
        total = 0;
        balance = 0;
    }
    
    /**
     * Return the price of a ticket.
     */

    public int getPrice()
    {
        return price;
    }

    /**
     * Return the amount of money already inserted for the
     * next ticket.
     */

    public int getBalance()
    {
        return balance;
    }

    /**
     * Receive an amount of money in cents from a customer.
     * @param amount The amount of money to be inserted.
     */

    public void insertMoney(int amount)
    {
        balance = balance + amount;
    }

    /**
     * Returns the total amount of money paid for tickets after the machine was last emptied.
     */

    public int returnTotal()
    {
        return total;
    }

    /**
     * Sets new price of tickets.
     * @param newPrice The new price of tickets.
     */

    public void setPrice(int newPrice)
    {
        price = newPrice;
    }

    /**
     * Sets a discount for tickets. Reducing the price per ticket by amount.
     * @param amount The amount of cents to be subtracted from price.
     */

    public void discount(int amount)
    {
        price -= amount;
    }

    /**
     * Prompts for the correct amount of money.
     */

    public void prompt()
    {
        System.out.println("Please insert the correct amount of money.");
    }

    /**
     * Prints out the current price for a ticket.
     */

    public void printPrice()
    {
        System.out.println("The price of a ticket is " + price + " cents.");
    }

    /**
     * Print a ticket.
     * Update the total collected and
     * reduce the balance to zero.
     */
    public void printTicket()
    {
        // Simulate the printing of a ticket.
        System.out.println("##################");
        System.out.println("# The BlueJ Line");
        System.out.println("# Ticket");
        System.out.println("# " + price + " cents.");
        System.out.println("##################");
        System.out.println();

        // Update the total collected with the balance.
        total = total + balance;
        // Clear the balance.
        balance = 0;
    }
}
