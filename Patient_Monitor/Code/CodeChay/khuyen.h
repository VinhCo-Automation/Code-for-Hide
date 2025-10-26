#pragma once

void khuyen(int spo2, int bpm) {
    if(spo2 >= 95){
        if(bpm >= 60 && bpm <= 100){
            Blynk.virtualWrite(V3, "Tốt. Duy trì theo dõi. Thư giãn và tiếp tục sinh hoạt bình thường.");
        } else if(bpm > 100){
            Blynk.virtualWrite(V3, "Có thể do căng thẳng, sốt, mất nước, hoặc lo lắng.");
        } else {
            Blynk.virtualWrite(V3, "Thường gặp ở vận động viên.");
        }
    }else if (spo2 >= 90 && spo2 < 95){
        if(bpm >= 60 && bpm <= 100){
            Blynk.virtualWrite(V3, "Cảnh báo thiếu oxy máu.");
        } else if(bpm > 100){
            Blynk.virtualWrite(V3, "Báo động thiếu oxy máu.");
        } else {
            Blynk.virtualWrite(V3, "Tình trạng nguy hiểm.");
        }
    }else {
        Blynk.virtualWrite(V3, "Cấp cứu y tế ngay lập tức!");
    }
    
}