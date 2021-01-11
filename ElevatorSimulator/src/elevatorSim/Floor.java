package elevatorSim;

import exceptions.InvalidParamException;
import exceptions.InvalidStateException;

import java.util.ArrayList;
import java.util.Iterator;

public class Floor {
    private static int lastFloor = 1;
    private int floorNum;
    private ArrayList<Person> peopleWaiting = new ArrayList<>();
    private ArrayList<Person> peopleDone = new ArrayList<>();

    public Floor(){
        floorNum = lastFloor;
        lastFloor = floorNum + 1;
    }

    //Get the people waiting on floor who are going in same direction as elevator
    public ArrayList<Person> getPeopleInDirection(Direction d) throws InvalidParamException {
        if (d == Direction.IDLE) {
            throw new InvalidParamException("Invalid direction in getPeopleInDirection");
        }
        ArrayList<Person> peopleInDirection = new ArrayList<>();
        for (Iterator<Person> itr = peopleWaiting.iterator(); itr.hasNext();) {
            Person p = itr.next();
            if (p.getDirection() == d) {
                peopleInDirection.add(p);
                itr.remove();

                System.out.println(Output.timeStamp() + " " + p.toString() + " has left Floor " + getFloorNum());
            }
        }
        return peopleInDirection;
    }

    //Add a person to the wait line
    public void addToWait(Person p) throws InvalidStateException {
        if (waitingPersonExists(p)) {
            throw new InvalidStateException("Invalid state: person added to wait already exists");
        }
        peopleWaiting.add(p);
    }

    //Add a person to the done line
    public void addToDone(Person p) throws InvalidStateException {
        if (donePersonExists(p)) {
            throw new InvalidStateException("Invalid state: person added to done already exists");
        }
        peopleDone.add(p);

        System.out.println(Output.timeStamp() + " " + p.toString() + " entered Floor " + getFloorNum());
    }

    public int getFloorNum() {
        return floorNum;
    }

    public ArrayList<PersonDTO> getDonePeople() {
        ArrayList<PersonDTO> done = new ArrayList<>();
        for (Person p : peopleDone) {
            PersonDTO temp = new PersonDTO(p.getId(), p.getStartFloor(), p.getEndFloor(),
                    p.getDirection(), p.getWaitTime(), p.getRideTime());
            done.add(temp);
        }
        return done;
    }

    private boolean waitingPersonExists(Person p) {
        for (Person waiting : peopleWaiting) {
            if (waiting.getId() == p.getId()) {
                return true;
            }
        }
        return false;
    }

    private boolean donePersonExists(Person p) {
        for (Person done : peopleDone) {
            if (done.getId() == p.getId()) {
                return true;
            }
        }
        return false;
    }

    public String toString() {
        return "Floor " + floorNum;
    }
}
