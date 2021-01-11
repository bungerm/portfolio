package elevatorSim;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;

import exceptions.InvalidParamException;
import exceptions.InvalidStateException;
import gui.ElevatorDisplay;
import static gui.ElevatorDisplay.Direction.*;

public class Elevator{
    private static int lastId = 1;
    private int id;
    private ArrayList<Person> riders = new ArrayList<>();
    private int currentFloor;
    private Direction direction;
    private ArrayList<Request> floorRequests = new ArrayList<>();
    private ArrayList<Request> riderRequests = new ArrayList<>();
    private boolean doorOpen;

    // These are fed in by file
    private long floorTime;
    private long doorTime;
    private long idleTime;
    private int capacity;

    //Timer counts
    private long floorCount;
    private long doorCount;
    private long idleCount;

    //Constructor parameters have been error checked before they get here
    public Elevator(int capacityIn, long floorTimeIn, long doorTimeIn, long idleTimeIn) throws InvalidParamException {
        if (capacityIn < 1) {
            throw new InvalidParamException("Invalid capacity in Elevator c'tor");
        }
        if (floorTimeIn <= 0) {
            throw new InvalidParamException("Invalid floorTime in Elevator c'tor");
        }
        if (doorTimeIn <= 0) {
            throw new InvalidParamException("Invalid doorTime in Elevator c'tor");
        }
        if (idleTimeIn <= 0) {
            throw new InvalidParamException("Invalid idleTime in Elevator c'tor");
        }
        id = lastId;
        lastId = id + 1;
        currentFloor = 1;
        direction = Direction.IDLE;

        capacity = capacityIn;
        floorTime = floorTimeIn;
        doorTime = doorTimeIn;
        idleTime = idleTimeIn;

        floorCount = floorTime;
        doorCount = doorTime;
        idleCount = idleTime;
    }

    //When elevator reaches a destination floor, loop through riders to see who wants off,
    //unload them and remove their rider requests.
    private void loadElevator() throws InvalidParamException {
        ArrayList<Person> newRiders = Building.getInstance().getNewRiders(getCurrentFloor(), getDirection());
        if (!newRiders.isEmpty()) {
            for (Person rider : newRiders) {
                if (riders.size() < capacity ) {
                    riders.add(rider);
                    rider.setWaitTimeEnd();
                    rider.setRideTimeStart();
                    Request newRequest = new Request(rider.getEndFloor());
                    riderRequests.add(newRequest);

                    //output message 8
                    System.out.println(Output.timeStamp() + " " + rider.toString() + " has entered "
                            + toString() + " [Riders: " + riders.toString() + "]");

                    //output message 4
                    System.out.println(Output.timeStamp() + " " + toString() + " Rider Request made for Floor "
                            + newRequest.getFloor() + " [Current Floor Requests: " + floorRequests.toString()
                            + "][Current Rider Requests: " + riderRequests.toString() + "]");
                }
            }
        }
        for (Iterator<Request> itr = floorRequests.iterator(); itr.hasNext();) {
            Request request = itr.next();
            if (request.getFloor() == getCurrentFloor() && request.getDirection() == getDirection() ) {
                itr.remove();
            }
        }
    }

    //When elevator reaches a destination floor, loop through people on floor for anyone going in
    //this direction, load them and add their rider requests. Remove associated floor requests.
    private void unloadElevator() throws InvalidParamException, InvalidStateException {
        for (Iterator<Person> itr = riders.iterator(); itr.hasNext();) {
            Person person = itr.next();
            if (getCurrentFloor() == person.getEndFloor()) {
                Building.getInstance().addDone(getCurrentFloor(), person);
                person.setRideTimeEnd();
                itr.remove();

                //output message 9
                System.out.println(Output.timeStamp() + " " + person.toString() + " has left "
                        + toString() + " [Riders: " + riders.toString() + "]");
            }
        }
        for (Iterator<Request> itr = riderRequests.iterator(); itr.hasNext();) {
            Request request = itr.next();
            if (request.getFloor() == getCurrentFloor()) {
                itr.remove();
            }
        }
    }

    //Add a floor request
    //The parameter has been error checked higher up
    public void addFloorRequest(Request request) throws InvalidParamException {
        if (request == null) {
            throw new InvalidParamException("Null request given in addFloorRequeest");
        }
        if (getDirection() == Direction.IDLE){
            int temp = request.getFloor() - getCurrentFloor();
            if (temp < 0) {
                direction = Direction.DOWN;
            } else if (temp > 0){
                direction = Direction.UP;
            } else {
                direction = request.getDirection();
            }
        }
        floorRequests.add(request);
        idleCount = idleTime;

        //output message 3
        System.out.println(Output.timeStamp() + " " + toString() + " is going to Floor " + request.getFloor()
                + " for " + request.getDirection() + " request [Current Floor Requests: "
                + floorRequests.toString() + "][Current Rider Requests: " + riderRequests.toString() + "]");
    }

    //The main behaviour logic behind the elevators
    //If the door is open, count down close
    //else if there are requests, handle them
    //else elevator is idle
    public void update(long time) throws InvalidParamException, InvalidStateException {
        if (time <= 0) {
            throw new InvalidParamException("Invalid elevator update time");
        }
        if (doorOpen) {
            doorCount = doorCount - time;
            if (doorCount <= 0.0) {
                closeDoor();
                updateDisplay();
            }
        } else if (!riderRequests.isEmpty() || !floorRequests.isEmpty()) {
            updateDirection();
            if (checkRequests()) {
                openDoor();
                unloadElevator();
                loadElevator();
            } else {
                move();
            }
        } else {
            direction = Direction.IDLE;
            updateDisplay();
            ArrayList<Request> requests = ElevatorController.getInstance().getPendingRequests(getId());
            if (!requests.isEmpty()) {
                for (Request r : requests) {
                    addFloorRequest(r);
                }
            } else {
                if (idleCount > 0.0) {
                    idleCount = idleCount - time;
                } else if (getCurrentFloor() != 1){
                    move();
                }
            }
        }
    }

    private void updateDisplay() {
        if (getDirection() == Direction.UP) {
            ElevatorDisplay.getInstance().updateElevator(getId(), getCurrentFloor(), getRiders().size(), UP);
        } else if (getDirection() == Direction.DOWN) {
            ElevatorDisplay.getInstance().updateElevator(getId(), getCurrentFloor(), getRiders().size(), DOWN);
        } else {
            ElevatorDisplay.getInstance().updateElevator(getId(), getCurrentFloor(), getRiders().size(), IDLE);
            ElevatorDisplay.getInstance().setIdle(getId());
        }
    }

    //Move the elevator in the current direction (or appropriate direction if idle)
    private void move() {
        Direction myDir = getDirection();
        int oldFloor = getCurrentFloor();
        if (myDir == Direction.DOWN) {
            currentFloor = currentFloor - 1;
        } else if (myDir == Direction.UP) {
            currentFloor = currentFloor + 1;
        } else if (getCurrentFloor() != 1) {
            currentFloor = currentFloor - 1;
        }
        updateDisplay();

        //output message 5

        System.out.println(Output.timeStamp() + " " + toString() + " moving from Floor " + oldFloor
                + " to Floor " + getCurrentFloor() + " [Current Floor Requests: " + floorRequests.toString()
                + "][Current Rider Requests: " + riderRequests.toString() + "]");

    }

    //Check to see if any requests are concerning the current floor
    private boolean checkRequests() {
        for (Request request : riderRequests) {
            if (request.getFloor() == getCurrentFloor()) {

                //output message 6
                System.out.println(Output.timeStamp() + " " + toString() + " has arrived at Floor "
                        + getCurrentFloor() + " for Rider Request [Current Floor Requests: " + floorRequests.toString()
                        + "][Current Rider Requests: " + riderRequests.toString() + "]");

                return true;
            }
        }
        for (Request request : floorRequests) {
            if (request.getFloor() == getCurrentFloor() && request.getDirection() == getDirection()) {

                //output message 6
                System.out.println(Output.timeStamp() + " " + toString() + " has arrived at Floor "
                        + getCurrentFloor() + " for Floor Request [Current Floor Requests: " + floorRequests.toString()
                        + "][Current Rider Requests: " + riderRequests.toString() + "]");

                return true;
            }
        }
        return false;
    }

    //if there are still riders, or floor requests further in your direction, continue
    //otherwise, flip direction
    private void updateDirection() {
        if (!riders.isEmpty()) {
            return;
        }
        for (Request request : floorRequests) {
            if ((request.getFloor() > getCurrentFloor() && getDirection() == Direction.UP)
                    || (request.getFloor() < getCurrentFloor() && getDirection() == Direction.DOWN)
                    || (request.getFloor() == getCurrentFloor() && getDirection() == request.getDirection())) {
                return;
            }
        }
        if (getDirection() == Direction.UP) {
            direction = Direction.DOWN;
        } else {
            direction = Direction.UP;
        }
    }

    //Open the door
    private void openDoor() {
        doorOpen = true;
        doorCount = doorTime;

        //output message 7
        System.out.println(Output.timeStamp() + " " + toString() + " Doors Open");

        ElevatorDisplay.getInstance().openDoors(getId());
    }

    //Close the door
    private void closeDoor() {
        doorOpen = false;

        //output message 7
        System.out.println(Output.timeStamp() + " " + toString() + " Doors Close");

        ElevatorDisplay.getInstance().closeDoors(getId());
    }

    //Get the current elevator riders
    private ArrayList<Person> getRiders() {
        return riders;
    }

    public int getCurrentFloor() {
        return currentFloor;
    }

    public Direction getDirection() {
        return direction;
    }

    public int getId() {
        return id;
    }

    public String toString() {
        return "Elevator " + getId();
    }
}
