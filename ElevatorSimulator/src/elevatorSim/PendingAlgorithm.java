package elevatorSim;

import java.util.ArrayList;

public interface PendingAlgorithm {
    public ArrayList<Request> getPendingRequests(ArrayList<Request> pendingRequests, int floor);
}
