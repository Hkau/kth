
/**
 * Heater simply represents a heater.
 * It stores a temperature which can be manipulated by calling cooler or warmer, functions within the class.
 * 
 * @author Peter Boström
 * @version 1
 */
public class Heater
{
    // Current temperature, increment/decrement value and the max/min temperature that Heater can produce.
    private int temperature, increment, max, min;

    /**
     * Constructs a Heater. Heater defaults to a temperature of 15 degrees and sets to increment by 5.
     * setMin and setMax are used to limit the temperature range of the Heater.
     * @param setMin Minimal temperature that can be produced. Should be less than the default temperature.
     * @param setMax Maximal temperature that can be produced. Should be more than the default temperature.
     */

    public Heater(int setMin, int setMax)
    {
        min = setMin;
        max = setMax;
        temperature = 15;
        increment = 5;
    }

	/**
	 * Returns the Heater's current temperature.
	 */

    public int getTemperature()
    {
        return temperature;
    }
    
    /**
     * Increases temperature by increment. Limits the temperature to max.
     */
    public void warmer()
    {
        temperature += increment;
        if(temperature > max)
            temperature = max;
    }
    
    /**
     * Decreases temperature by increment. Limits the temperature to min.
     */
    public void cooler()
    {
        temperature -= increment;
        if(temperature < min)
            temperature = min;
    }
    /**
     * Sets a new temperature increment. Used to determine the steps between temperature used by cooler and warmer.
     * @param newIncrement New increment value to be used.
     */
    public void setIncrement(int newIncrement)
    {
	if(increment > 0)
        	increment = newIncrement;
    }
}
