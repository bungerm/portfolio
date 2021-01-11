package elevatorSim;

import java.util.ArrayList;
import java.util.Iterator;

public class PendingAlgorithmBaseImpl implements PendingAlgorithm {
    @Override
    public ArrayList<Request> getPendingRequests(ArrayList<Request> pendingRequests, int floor) {
        ArrayList<Request> goodRequests = new ArrayList<>();
        if (!pendingRequests.isEmpty()) {
            goodRequests.add(pendingRequests.remove(0));
            Request firstRequest = goodRequests.get(0);
            for (Iterator<Request> itr = pendingRequests.iterator(); itr.hasNext();) {
                Request request = itr.next();
                if (firstRequest.getDirection() == request.getDirection()) {
                    if (firstRequest.getDirection() == Direction.UP && firstRequest.getFloor() < request.getFloor()) {
                        goodRequests.add(request);
                        itr.remove();
                    }
                    if (firstRequest.getDirection() == Direction.DOWN && firstRequest.getFloor() > request.getFloor()) {
                        goodRequests.add(request);
                        itr.remove();
                    }
                }
            }
        }
        return goodRequests;
    }
}
