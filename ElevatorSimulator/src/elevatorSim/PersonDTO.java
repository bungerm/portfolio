package elevatorSim;

public class PersonDTO {
    public int id;
    public int startFloor;
    public int endFloor;
    public Direction direction;
    public double waitTime;
    public double rideTime;

    public PersonDTO(int idIn, int startFloorIn, int endFloorIn, Direction directionIn, double waitTimeIn, double rideTimeIn) {
        id = idIn;
        startFloor = startFloorIn;
        endFloor = endFloorIn;
        direction = directionIn;
        waitTime = waitTimeIn;
        rideTime = rideTimeIn;
    }
}
