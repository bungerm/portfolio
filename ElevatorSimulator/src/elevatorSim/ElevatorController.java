package elevatorSim;

import exceptions.InvalidParamException;
import exceptions.InvalidStateException;

import java.util.ArrayList;

public class ElevatorController {
    private int numElevators;
    private int capacity;
    private long floorTime;
    private long doorTime;
    private long idleTime;
    private ArrayList<Elevator> elevators = new ArrayList<>();
    private ArrayList<Request> pendingRequests = new ArrayList<>();
    private ElevatorAlgorithm elevatorAlgorithm;
    private PendingAlgorithm pendingAlgorithm;

    private static ElevatorController instance;

    private ElevatorController() {}

    public static ElevatorController getInstance() {
        if (instance == null) {
            instance = new ElevatorController();
        }
        return instance;
    }

    //Initialize the elevator controller variables and create all elevators
    public void initialize(int numElevatorsIn, int capacityIn, long floorTimeIn, long doorTimeIn, long idleTimeIn, String eaTypeIn, String paTypeIn) {
        try {
            setNumElevators(numElevatorsIn);
            setCapacity(capacityIn);
            setFloorTime(floorTimeIn);
            setDoorTime(doorTimeIn);
            setIdleTime(idleTimeIn);

            for (int i = 1; i < numElevators+1; i++) {
                Elevator newElevator = new Elevator(capacity, floorTime, doorTime, idleTime);
                elevators.add(newElevator);
            }

            elevatorAlgorithm = ElevatorAlgorithmFactory.createElevatorAlgorithm(eaTypeIn);
            pendingAlgorithm = PendingAlgorithmFactory.createPendingAlgorithm(paTypeIn);
        } catch(InvalidParamException e) {
            System.out.println(e.getMessage());
            System.exit(1);
        }
    }

    private void setNumElevators(int numElevatorsIn) throws InvalidParamException {
        if (numElevatorsIn < 1){
            throw new InvalidParamException("Invalid number of elevators input");
        } else {
            this.numElevators = numElevatorsIn;
        }
    }

    private void setCapacity(int capacityIn) throws InvalidParamException {
        if (capacityIn < 1){
            throw new InvalidParamException("Invalid capacity input");
        } else {
            this.capacity = capacityIn;
        }
    }

    private void setFloorTime(long floorTimeIn) throws InvalidParamException {
        if (floorTimeIn <= 0.0){
            throw new InvalidParamException("Invalid floor time input");
        } else {
            this.floorTime = floorTimeIn;
        }
    }

    private void setDoorTime(long doorTimeIn) throws InvalidParamException {
        if (doorTimeIn <= 0.0){
            throw new InvalidParamException("Invalid door time input");
        } else {
            this.doorTime = doorTimeIn;
        }
    }

    private void setIdleTime(long idleTimeIn) throws InvalidParamException {
        if (idleTimeIn <= 0.0){
            throw new InvalidParamException("Invalid idle time input");
        } else {
            this.idleTime = idleTimeIn;
        }
    }

    //Get an elevator by its id
    private Elevator getElevator(int id) throws InvalidParamException {
        if (id < 1 || id > elevators.size()) {
            throw new InvalidParamException("Invalid ID in getElevator");
        }
        Elevator elevator = null;
        for (Elevator e : elevators) {
            if (e.getId() == id) {
                elevator = e;
            }
        }
        return elevator;
     }

     private void addFloorRequestToElevator(int elevatorNum, Request request) throws InvalidParamException {
        getElevator(elevatorNum).addFloorRequest(request);
     }

    //Tell all the elevators to update themselves
    public void updateElevators(long time) throws InvalidParamException, InvalidStateException {
        if (time <= 0) {
            throw new InvalidParamException("Invalid elevator update time");
        }
        for (Elevator e : elevators) {
            e.update(time);
        }
    }

    //Check to see if all elevators are idle
    public boolean idleElevators() {
        for (Elevator e : elevators) {
            if (e.getDirection() != Direction.IDLE || e.getCurrentFloor()!= 1) {
                return false;
            }
        }
        return true;
    }

    public void addFloorRequest(Request request) throws InvalidParamException {
        if (request == null) {
            throw new InvalidParamException("Invalid request in addFloorRequest");
        }
        int elevatorNum = elevatorAlgorithm.findElevator(elevators, request);
        if (elevatorNum == 0) {
            pendingRequests.add(request);
        } else {
            addFloorRequestToElevator(elevatorNum, request);
        }
    }

    public ArrayList<Request> getPendingRequests(int floorNum) throws InvalidParamException {
        if (floorNum < 1 || floorNum > elevators.size()) {
            throw new InvalidParamException("Invalid floor number in getPendingRequests");
        }
        return pendingAlgorithm.getPendingRequests(pendingRequests, floorNum);
    }
}
