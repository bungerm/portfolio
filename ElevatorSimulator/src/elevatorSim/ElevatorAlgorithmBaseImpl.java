package elevatorSim;

import java.util.ArrayList;

public class ElevatorAlgorithmBaseImpl implements ElevatorAlgorithm {
    @Override
    public int findElevator(ArrayList<Elevator> elevators, Request r) {
        for (Elevator e : elevators) {
            if (e.getCurrentFloor() == r.getFloor()) {
                if (e.getDirection() == Direction.IDLE || e.getDirection() == r.getDirection()) {
                    return e.getId();
                }
            }
        }
        for (Elevator e : elevators) {
            if (e.getDirection() != Direction.IDLE) {
                if (desiredDirection(e, r)) {
                    return e.getId();
                }
            }
        }
        for (Elevator e : elevators) {
            if (e.getDirection() == Direction.IDLE) {
                return e.getId();
            }
        }
        return 0;
    }

    private boolean desiredDirection(Elevator e, Request r) {
        if (e.getDirection() == r.getDirection()) {
            if (e.getDirection() == Direction.UP && e.getCurrentFloor() < r.getFloor()) {
                return true;
            } else if (e.getDirection() == Direction.DOWN && e.getCurrentFloor() > r.getFloor()) {
                return true;
            }
        }
        return false;
    }
}
