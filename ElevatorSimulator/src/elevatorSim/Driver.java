package elevatorSim;

import exceptions.InvalidParamException;
import exceptions.InvalidStateException;
import gui.ElevatorDisplay;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Random;

public class Driver {
    private static Random randObj = new Random(System.currentTimeMillis());

    public static void main(String[] args) {

        FileReader reader;
        try {
            // Create a FileReader object using your filename
            reader = new FileReader("input.json");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return;
        }
        JSONParser jsonParser = new JSONParser();
        JSONObject jObj;

        try {
            // Create a JSONParser using the FileReader
            jObj = (JSONObject) jsonParser.parse(reader);
        } catch (IOException | ParseException e) {
            e.printStackTrace();
            return;
        }

        Long numFloorsLong = (Long) jObj.get("floors");
        Long numElevatorsLong = (Long) jObj.get("elevators");
        Long capacityLong = (Long) jObj.get("capacity");
        int numFloors = numFloorsLong.intValue();
        int numElevators = numElevatorsLong.intValue();
        int capacity = capacityLong.intValue();
        Long floorTime = (Long) jObj.get("floorTime");
        Long doorTime = (Long) jObj.get("doorTime");
        Long idleTime = (Long) jObj.get("idleTime");
        Long simTimeLong = (Long) jObj.get("simTime");
        Long personRateLong = (Long) jObj.get("personRate");
        String elevatorAlgorithmType = (String) jObj.get("elevatorAlgorithm");
        String pendingAlgorithmType = (String) jObj.get("pendingAlgorithm");

        try {
            if (simTimeLong <= 0.0) {
                throw new InvalidParamException("Invalid simulation time input");
            }
            if (personRateLong <= 0.0) {
                throw new InvalidParamException("Invalid person rate input");
            }
        } catch (InvalidParamException e) {
            System.out.println(e.getMessage());
            System.exit(1);
        }

        int simTime = simTimeLong.intValue();
        int personRate = personRateLong.intValue();

        Building.getInstance().initialize(numFloors, numElevators, capacity, floorTime, doorTime, idleTime,
                elevatorAlgorithmType, pendingAlgorithmType);

        ElevatorDisplay.getInstance().initialize(numFloors);
        for (int i = 1; i <= numElevators; i++) {
            ElevatorDisplay.getInstance().addElevator(i, 1);
        }


        //RUN HERE
        try {
            for (int i = 0; i < simTime; i++) {
                if (i % personRate == 0) {
                    int startFloor = (int) (randObj.nextDouble() * Building.getInstance().getNumFloors() + 1);
                    int endFloor = (int) (randObj.nextDouble() * Building.getInstance().getNumFloors() + 1);
                    while (startFloor == endFloor) {
                        endFloor = (int) (randObj.nextDouble() * Building.getInstance().getNumFloors() + 1);
                    }
                    Person p = new Person(startFloor, endFloor);
                    Building.getInstance().addPerson(p);
                }

                ElevatorController.getInstance().updateElevators(1000);
                Thread.sleep(1000);
            }
            while (!ElevatorController.getInstance().idleElevators()) {
                ElevatorController.getInstance().updateElevators(1000);
                Thread.sleep(1000);
            }
            Building.getInstance().printStatistics();

        } catch (InvalidParamException | InterruptedException | InvalidStateException e) {
            e.printStackTrace();
            System.out.println(e.getMessage());
            System.exit(1);
        }

        ElevatorDisplay.getInstance().shutdown();
    }
}
