
public class Test {


    private static Test thisObject;

	
	
	
	public static void main (String[] args) { 
		Test obj = new Test();
		obj.createMystery();
		obj.createMystery();
		obj.createMystery();
		obj.createMystery();
		obj.createMystery();
		obj.createMystery();
		obj.createMystery();
		obj.createMystery();
		obj.createMystery();
		obj.createMystery();
		
		
	}
	
	
	



	    private Test() {
	    }

	    public static Test createMystery() {
	        if (thisObject == null)
		    thisObject = new Test();
	        return thisObject;
	    }
	}


	


