package elevatorSim;

import exceptions.InvalidParamException;

public class ElevatorAlgorithmFactory {

    private ElevatorAlgorithmFactory() {}

    public static ElevatorAlgorithm createElevatorAlgorithm(String type) throws InvalidParamException {
        if (type.equals("base")) {
            return new ElevatorAlgorithmBaseImpl();
        } else {
            throw new InvalidParamException("Invalid elevator algorithm type");
        }
    }
}
