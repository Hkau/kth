/*
A vehicle repair station can repair vehicles of three different types A, B and C. The station has the following capacity:

    * It can repair in parallel at most a, b, c vehicles of type A, B, C, correspondingly;
        * It can repair in parallel at most v vehicles of different types.

	If a vehicle cannot be repaired because any (or both) of the above limitations, the vehicle has to wait until it can get a place to be repaired.

	(a) Develop a concurrent object class (a monitor with condition variables) that controls the station so that several vehicles can be repaired in parallel according to the capacity of the station as described above. Define the monitor's operations and their implementation. Represent vehicles as processes and show how they can use the monitor. Is your solution fair? Explain (in comments in the source code).
	(b) Implement a multithreaded application in Java that simulates the actions of the vehicles represented as concurrent threads and the repair station represented as as the monitor. Assume that each vehicle periodically arrives at the station to get repaired. Have the vehicles sleep (pause) for a random amount of time between arriving at the station to simulate the time it takes to travel; and have the vehicles sleep (pause) for a smaller random amount of time to simulate the time it takes to be repaired at the station. Stop the simulation after each vehicle has arrived at the station trips (the given amount) times. Your program should print a trace of the interesting events in the program. 

*/

import java.util.Random;
import java.lang.Thread;

public class repair
{
	private final static int TRIPS = 5;
	private static Random rand = new Random();
	private final static int CAR_TYPES = 3;

	// Important shared shop
	private static Shop shop;


	// This is the monitor! :)
	private static class Shop
	{
		// This small class is where all the action happens.
		private class SyncLock
		{
			private int limit;
			private int count;

			public SyncLock(int limit)
			{
				this.limit = limit;
			}

			public void increase()
			{
				while(true)
				{
					synchronized(this)
					{
						if(count < limit)
						{
							++count;
							break;
						}
						try
						{
							wait();
						}
						catch(InterruptedException e)
						{
						}
					}
				}
			}
			public void decrease()
			{
				synchronized(this)
				{
					if(count-- == limit)
						notify();
				}
			}
		}

		// Important slots for each type
		private SyncLock[] types;
		private SyncLock total;

		public Shop(int totalLimit, int... limits)
		{
			types = new SyncLock[CAR_TYPES];
			assert limits.length == CAR_TYPES;

			for(int i = 0; i < CAR_TYPES; ++i)
				types[i] = new SyncLock(limits[i]);

			total = new SyncLock(totalLimit);
		}

		public void HoldSlot(int type)
		{
			// Each car must acquire both a slot for its type
			types[type].increase();
			// And one of the global slots
			total.increase();
		}

		public void FreeSlot(int type)
		{
			// Finally they must be given back
			total.decrease();
			types[type].decrease();
		}
	}

	// This is each thread.
	private static class Car extends Thread
	{
		private int id;
		private int type;

		public Car(int id, int type)
		{
			this.id = id;
			this.type = type;
		}

		public void sleep(int ms)
		{
			try
			{
				Thread.sleep(ms);
			}
			catch(InterruptedException e)
			{
			}
		}
		public void run()
		{
			int trips = TRIPS;
			while(trips-- != 0)
			{
				System.out.printf("CAR %d (type %c): Wroom!\n", id, 'A' + type);
				sleep(10 + rand.nextInt(100));
				shop.HoldSlot(type);
				System.out.printf("CAR %d (type %c): I just got a slot! Phew!\n", id, 'A'+type);
				sleep(100);
				System.out.printf("CAR %d (type %c): Sure feels good to get repaired!\n", id, 'A' + type);
				sleep(100 + rand.nextInt(500));
				shop.FreeSlot(type);
			}
			System.out.printf("CAR %d (type %c): But now I'm tired. I think I'll just drive home to my garage. :(\n", id, 'A' + type);
		}
	}

	public static void main(String[] args)
	{
		// Feel free to tweak these parameters.
		// For instance:
		// a = 1, b = 100, c = 100, v = 200
		// And a higher car count will demonstrate
		// that car types B and C will get repaired
		// quickly, while A will have to wait for
		// and A slot.
		int a = 1, b = 2, c = 5;
		int v = 6;

		int cars = 10;

		shop = new Shop(v, a, b, c);

		for(int i = 0; i < cars; ++i)
		{
			Car car = new Car(i, rand.nextInt(CAR_TYPES));
			car.start();
		}

	}
}

