package elevatorSim;

import exceptions.InvalidParamException;

public class PendingAlgorithmFactory {

    private PendingAlgorithmFactory() {}

    public static PendingAlgorithm createPendingAlgorithm(String type) throws InvalidParamException {
        if (type.equals("base")) {
            return new PendingAlgorithmBaseImpl();
        } else {
            throw new InvalidParamException("Invalid pending algorithm type");
        }
    }
}
