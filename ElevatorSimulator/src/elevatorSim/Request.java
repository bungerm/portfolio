package elevatorSim;

import exceptions.InvalidParamException;

public class Request {
    private int floor;
    private Direction direction;

    //This COULD be an interface with two different request implementations (rider and floor)
    //but SHOULD it be?
    public Request(int floorIn) throws InvalidParamException {
        if (floorIn < 1 || floorIn > Building.getInstance().getNumFloors()) {
            throw new InvalidParamException("Invalid floorIn in Request c'tor");
        }
        floor = floorIn;
        direction = null;
    }

    public Request(int floorIn, Direction directionIn) throws InvalidParamException {
        if (floorIn < 1 || floorIn > Building.getInstance().getNumFloors()) {
            throw new InvalidParamException("Invalid floorIn in Request c'tor");
        }
        if (directionIn == null) {
            throw new InvalidParamException("Null direction in Request c'tor");
        }
        floor = floorIn;
        direction = directionIn;
    }

    public int getFloor() {
        return floor;
    }

    public Direction getDirection() {
        return direction;
    }

    @Override
    public String toString() {
        return "" + floor;
    }
}
