package elevatorSim;

import exceptions.InvalidParamException;
import exceptions.InvalidStateException;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;

public class Building {
    private int numFloors;
    private ArrayList<Floor> floors = new ArrayList<>();
    private ElevatorController elevatorController = ElevatorController.getInstance();

    private static Building instance;

    private Building() {}

    public static Building getInstance(){
        if (instance == null){
            instance = new Building();
        }
        return instance;
    }

    //Initialize the building with file input
    public void initialize(int numFloorsIn, int numElevator, int capacity, long floorTime,
                           long doorTime, long idleTime, String eaType, String paType) {
        try {
            setNumFloors(numFloorsIn);
            for (int i = 1; i < numFloors+1; i++) {
                Floor newFloor = new Floor();
                floors.add(newFloor);
            }
            elevatorController.initialize(numElevator, capacity, floorTime, doorTime, idleTime, eaType, paType);
        } catch(InvalidParamException e) {
            System.out.println(e.getMessage());
            System.exit(1);
        }
    }

    public void addPerson(Person p) throws InvalidParamException, InvalidStateException {
        if (p == null) {
            throw  new InvalidParamException("Null Person in addPerson");
        }
        if (p.getStartFloor() < 1 || p.getStartFloor() > floors.size()) {
            throw new InvalidParamException("Invalid floor number in addPerson");
        }

        getFloor(p.getStartFloor()).addToWait(p);

        //output message 1
        System.out.println(Output.timeStamp() + " " + p.toString() + " created on Floor " + p.getStartFloor()
                + ", wants to go " + p.getDirection() + " to Floor " + p.getEndFloor());

        //output message 2
        System.out.println(Output.timeStamp() + " " + p.toString() + " pressed " + p.getDirection()
                + " button on Floor " + p.getStartFloor());
    }

    public int getNumFloors() {
        return numFloors;
    }

    //Get a certain floor
    private Floor getFloor(int floorNumIn) throws InvalidParamException {
        if (floorNumIn < 1 || floorNumIn > floors.size()) {
            throw new InvalidParamException("Invalid floor number in getFloor");
        }
        Floor myFloor = null;
        for (Floor f : floors) {
            if (f.getFloorNum() == floorNumIn) {
                myFloor = f;
            }
        }
        return myFloor;
    }

    //Set the number of floors
    private void setNumFloors(int numFloorsIn) throws InvalidParamException {
        if (numFloorsIn < 1) {
            throw new InvalidParamException("Invalid number of floors given");
        }
        this.numFloors = numFloorsIn;
    }

    public ArrayList<Person> getNewRiders(int floorNum, Direction d) throws InvalidParamException {
        return getFloor(floorNum).getPeopleInDirection(d);
    }

    public void addDone(int floorNum, Person p) throws InvalidParamException, InvalidStateException {
        Floor floor = getFloor(floorNum);
        floor.addToDone(p);
    }

    public void printStatistics() {
        ArrayList<PersonDTO> allDone = new ArrayList<>();
        for (Floor f : floors) {
            allDone.addAll(f.getDonePeople());
        }

        PersonDTO init = allDone.get(0);
        double avgWaitTime = 0;
        double avgRideTime = 0;
        double minWaitTime = init.waitTime;
        double maxWaitTime = init.waitTime;
        double minRideTime = init.rideTime;
        double maxRideTime = init.rideTime;
        int minWaitPersonID = init.id;
        int maxWaitPersonID = init.id;
        int minRidePersonID = init.id;
        int maxRidePersonID = init.id;

        for (PersonDTO p : allDone) {
            avgWaitTime += p.waitTime;
            avgRideTime += p.rideTime;
            if (p.waitTime < minWaitTime) {
                minWaitTime = p.waitTime;
                minWaitPersonID = p.id;
            }
            if (p.waitTime > maxWaitTime) {
                maxWaitTime = p.waitTime;
                maxWaitPersonID = p.id;
            }
            if (p.rideTime < minRideTime) {
                minRideTime = p.rideTime;
                minRidePersonID = p.id;
            }
            if (p.rideTime > maxRideTime) {
                maxRideTime = p.rideTime;
                maxRidePersonID = p.id;
            }
        }
        avgWaitTime /= allDone.size();
        avgRideTime /= allDone.size();

        System.out.println(String.format("\nAvg Wait Time: %.1f", avgWaitTime));
        System.out.println(String.format("Avg Ride Time: %.1f\n", avgRideTime));
        System.out.println(String.format("Min Wait Time: %.1f (P%d)", minWaitTime, minWaitPersonID));
        System.out.println(String.format("Min Ride Time: %.1f (P%d)\n", minRideTime, minRidePersonID));
        System.out.println(String.format("Max Wait Time: %.1f (P%d)", maxWaitTime, maxWaitPersonID));
        System.out.println(String.format("Max Ride Time: %.1f (P%d)\n", maxRideTime, maxRidePersonID));

        String[] s = new String[] {"Person", "Start Floor", "End Floor", "Direction", "Wait Time", "Ride Time", "Total Time"};
        String[] s2 = new String[] {"------", "-----------", "---------", "---------", "---------", "---------", "----------"};

        System.out.println(String.format("%s%15s%15s%15s%15s%15s%15s", s[0], s[1], s[2], s[3], s[4], s[5], s[6]));
        System.out.println(String.format("%s%15s%15s%15s%15s%15s%15s", s2[0], s2[1], s2[2], s2[3], s2[4], s2[5], s2[6]));

        for (PersonDTO p : allDone) {
            System.out.println(String.format("%6s%15d%15d%15s%15.1f%15.1f%15.1f",
                    "P" + p.id, p.startFloor, p.endFloor, p.direction, p.waitTime, p.rideTime, (p.rideTime+p.waitTime)));
        }
    }
}
