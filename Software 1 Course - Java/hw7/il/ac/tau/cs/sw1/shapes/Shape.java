package il.ac.tau.cs.sw1.shapes;


public interface Shape {
		 // Returns the shape's area 
		 public float getArea(); 
		 
		 // Returns the shape's perimeter 
		 public float getPerimeter(); 
		 
		 // Returns a string representation of the shape details. 
		 // The returned string would include the shape's name, its fields 
		 // and their corresponding values. 
		 public String getDetails(); 
}
