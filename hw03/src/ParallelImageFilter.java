import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveAction;

public class ParallelImageFilter extends ImageFilter {
    public ParallelImageFilter(String image) {
        super(image);
    }

    @Override
    public void applyMedianFilter() {
        long start = System.nanoTime();
        //////////////////////Fork join pool 선언 및 execute / join 활용	///////////////////////////////////////////////////////
        ForkJoinPool pool = new ForkJoinPool();
        ForkJoinAction action = new ForkJoinAction(image, filter_image, 0 ,this.width ,0, this.height);
        pool.execute(action);
        ////////////////////////////////////////////////////////////////////////////////////
        long end = System.nanoTime();
        System.out.println("Parallel Image Filter Median Filter Time : " + (end - start) / 1000000 + " ms");
    }

    private static class ForkJoinAction extends RecursiveAction {
        int[][] image;
        int[][] filter_image;
        private int x1, x2, y1, y2;

        ForkJoinAction(int[][] image, int[][] filter_image, int x1, int x2, int y1, int y2) {
            this.image = image;
            this.filter_image = filter_image;
            this.x1 = x1;
            this.x2 = x2;
            this.y1 = y1;
            this.y2 = y2;
        }

        /////// 위의 applyMedianFilter에서 execute 함수 사용 시 작동하는 함수
        /////// 최소 단위 (가로 * 세로의 값이 32)보다 작으면 중간 값 필터링 이용(ImageFilter.java의 applyMedianFilter 코드 이용)
        /////// 최소 단위보다 클 시 divideTask() 호출
        @Override
        protected void compute() {
            int x = x2 - x1; // 가로
            int y = y2 - y1; // 세로

            if (x * y >= 32) { // 가로 * 세로가 32보다 작지 않으면
                List<ForkJoinAction> subtask = divideTask();
                subtask.get(0).fork();
                subtask.get(1).compute();
                subtask.get(0).join();

            } else { //가로 * 세로가 32보다 작으면
                int i = y1;
                while (i < y2) {
                    for (int j = x1; j < x2; ++j) {
                        int rgb = getMedianValue(image, i, j);
                        filter_image[i][j] = rgb;
                    }
                    ++i;
                }
            }
        }

        ////// subtask 생성에 대한 부분 정의하는 곳 (subtaks.add 활용)
        ////// 세로가 가로보다 큰 경우, 세로를 반 나누어 subtask 내의 작업 생성
        ////// 가로가 세로보다 큰 경우, 가로를 반 나누어 subtask 내의 작업 생성
        private List<ForkJoinAction> divideTask() {
            List<ForkJoinAction> subTasks = new ArrayList<ForkJoinAction>();
            int x = x2 - x1; // 가로
            int y = y2 - y1; // 세로

            if (y > x) { // 세로가 가로보다 큰 경우
                subTasks.add(new ForkJoinAction(image, filter_image, x1, x2, (y2 + y1) / 2, y2));
                subTasks.add(new ForkJoinAction(image, filter_image, x1, x2, y1, (y2 + y1) / 2));
            } else { // 가로가 세로보다 큰 경우
                subTasks.add(new ForkJoinAction(image, filter_image, (x2 + x1) / 2, x2, y1, y2));
                subTasks.add(new ForkJoinAction(image, filter_image, x1, (x2 + x1) / 2, y1, y2));
            }
            return subTasks;
        }
    }
}
