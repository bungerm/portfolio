package elevatorSim;

import exceptions.InvalidParamException;

import java.lang.*;

public class Person {
    private static int lastId = 1;
    private int id;
    private int startFloor;
    private int endFloor;
    private Direction direction;
    private long waitTimeStart;
    private long waitTimeEnd;
    private long rideTimeStart;
    private long rideTimeEnd;

    //Constructor parameter error checked before reached here
    public Person(int startFloorIn, int endFloorIn) throws InvalidParamException {
        if (startFloorIn < 1 || startFloorIn > Building.getInstance().getNumFloors()) {
            throw new InvalidParamException("Invalid startFloor in Person c'tor");
        }
        if (endFloorIn < 1 || endFloorIn > Building.getInstance().getNumFloors()) {
            throw new InvalidParamException("Invalid endFloor in Person c'tor");
        }
        id = lastId ;
        lastId = id + 1;
        startFloor = startFloorIn;
        endFloor = endFloorIn;
        if (startFloor < endFloor) {
            direction = Direction.UP;
        } else {
            direction = Direction.DOWN;
        }
        waitTimeStart = System.currentTimeMillis();

        Request myRequest = new Request(getStartFloor(), getDirection());
        ElevatorController.getInstance().addFloorRequest(myRequest);
    }

    //Get the time spent waiting for elevator
    public double getWaitTime() {
        return (getWaitTimeEnd() - getWaitTimeStart())/1000.0;
    }

    //Get the time spent riding the elevator
    public double getRideTime() {
        return (getRideTimeEnd() - getRideTimeStart())/1000.0;
    }

    public void setWaitTimeEnd() {
        this.waitTimeEnd = System.currentTimeMillis();
    }

    public void setRideTimeStart() {
        this.rideTimeStart = System.currentTimeMillis();
    }

    public void setRideTimeEnd() {
        this.rideTimeEnd = System.currentTimeMillis();
    }

    public int getId() {
        return id;
    }

    public int getStartFloor() {
        return startFloor;
    }

    public int getEndFloor() {
        return endFloor;
    }

    public Direction getDirection() {
        return direction;
    }

    private long getWaitTimeStart() {
        return waitTimeStart;
    }

    private long getWaitTimeEnd() {
        return waitTimeEnd;
    }

    private long getRideTimeStart() {
        return rideTimeStart;
    }

    private long getRideTimeEnd() {
        return rideTimeEnd;
    }

    public String toString() {
        return "Person " + getId();
    }
}
