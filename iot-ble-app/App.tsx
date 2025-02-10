import React, { useEffect, useState } from 'react';
import { View, Text, StyleSheet, Dimensions, Image, ScrollView, ImageBackground, TouchableOpacity, FlatList } from 'react-native';
import { LineChart, CurveType } from 'react-native-gifted-charts';
import { Card } from 'react-native-paper';
import Animated, { useSharedValue, useAnimatedStyle, withTiming } from 'react-native-reanimated';
import { MaterialIcons } from '@expo/vector-icons';
import FontAwesome from '@expo/vector-icons/FontAwesome';
import { ref, onValue } from 'firebase/database';
import { database } from './firebaseConfig';

const screenWidth = Dimensions.get('window').width;

const App = () => {
  const [selectedTab, setSelectedTab] = useState<'Temperature' | 'Humidity'>('Temperature');
  const [sensorData, setSensorData] = useState([
    ...Array(1).fill({
      humidity: 50.0, // Đặt giá trị mặc định cho độ ẩm
      temperature: 25.0, // Đặt giá trị mặc định cho nhiệt độ
      time: new Date().toLocaleTimeString(), // Đặt thời gian hiện tại
      })
  ]); // Mảng lưu dữ liệu từ Firebase
  const [temperature, setTemperature] = useState<number | null>(null);
  const [humidity, setHumidity] = useState<number | null>(null);

  const [minTemp, setMinTemp] = useState(100.0);
  const [minHumi, setMinHumi] = useState(100.0);

  const indicatorPosition = useSharedValue(0);
  const handleTabPress = (tab: 'Temperature' | 'Humidity', index: number) => {
    setSelectedTab(tab);
    indicatorPosition.value = withTiming(index * (screenWidth / 2), { duration: 300 });
  };

  const handleRefresh = () => {
    console.log('Refresh button pressed');
    // Thêm logic làm mới tại đây nếu cần
  };
  
  const animatedIndicatorStyle = useAnimatedStyle(() => ({
    transform: [{ translateX: indicatorPosition.value }],
  }));

  useEffect(() => {
      const sensorDataRef = ref(database, 'sensorData/');
  
      // Lắng nghe sự thay đổi dữ liệu trong Firebase Realtime Database
      const unsubscribe = onValue(sensorDataRef, (snapshot) => {
        if (snapshot.exists()) {
          const data = snapshot.val();
          if (data) {
            setTemperature(data.temperature); // Cập nhật nhiệt độ
            setHumidity(data.humidity);       // Cập nhật độ ẩm
            if (data.temperature < minTemp) {
              setMinTemp(data.temperature);
            }
            if (data.humidity < minHumi) {
              setMinHumi(data.humidity);
            }
          }
          const newData = {
            time: data.time ?? new Date().toLocaleTimeString(), // Gán thời gian nếu không có
            temperature: data.temperature,
            humidity: data.humidity,
          };
          setSensorData((prevData) => {
            const updatedData = [...prevData, newData]; // Thêm dữ liệu mới vào cuối mảng
            if (updatedData.length > 30) {
              updatedData.shift(); // Xóa phần tử đầu tiên nếu mảng có hơn 20 phần tử
            }
            return updatedData; // Trả về mảng cập nhật
              });
            } else {
              console.log("No data available");
        
        }
      });
  
      // Dọn dẹp khi component bị hủy (unmount)
      return () => unsubscribe();
    }, []); // Mảng dependency rỗng, chỉ chạy một lần khi component được mount

    const chartData = {
      temperature: sensorData.map(item => item.temperature),
      humidity: sensorData.map(item => item.humidity),
    };
    // Dữ liệu của biểu đồ
    const dataTemp = chartData.temperature.map(value => ({ value }));  // Dữ liệu nhiệt độ dưới dạng [{ value: 20 }, { value: 21 }, ...]
    const dataHumi = chartData.humidity.map(value => ({ value }));  // Dữ liệu nhiệt độ dưới dạng [{ value: 20 }, { value: 21 }, ...]
  return (
    <ScrollView contentContainerStyle={styles.container}>
      {/* Nút refresh góc trên phải */}
      <Text style={styles.header}>Temperature and Humidity</Text>
      <ImageBackground
      source={require('./assets/bg.png')} // Đường dẫn tới ảnh trong thư mục assets
      style={styles.container1} // Đặt chiều cao bằng với container1
      resizeMode="cover" // Phủ kín container mà không để khoảng trống
      >
        <View style={styles.cardContainer}>
          <Card style={styles.card}>
            <View style={styles.cardContent}>
              <MaterialIcons name="thermostat" size={40} color="#007AFF" style={styles.icon} />
              <View>
                <Text style={styles.cardValue}>{temperature !== null ? `${temperature}°C` : 'Loading...'}</Text>
                <Text style={styles.cardStatus}>Comfortable</Text>
              </View>
            </View>
          </Card>

          <Card style={styles.card}>
            <View style={styles.cardContent}>
              <MaterialIcons name="water-drop" size={40} color="#007AFF" style={styles.icon} />
              <View>
                <Text style={styles.cardValue}>{humidity !== null ? `${humidity}%` : 'Loading...'}</Text>
                <Text style={styles.cardStatus}>Slightly dry</Text>
              </View>
            </View>
          </Card>
        </View>
      </ImageBackground>


      <View style={styles.tabContainer}>
        {/* Thanh chỉ báo */}
        <Animated.View style={[styles.indicator, animatedIndicatorStyle]} />

        {/* Tab Temperature */}
        <TouchableOpacity
          style={styles.tab}
          onPress={() => handleTabPress('Temperature', 0)}
        >
          <Text style={[styles.tabText, selectedTab === 'Temperature' && styles.activeTabText]}>
            Temperature
          </Text>
        </TouchableOpacity>

        {/* Tab Humidity */}
        <TouchableOpacity
          style={styles.tab}
          onPress={() => handleTabPress('Humidity', 1)}
        >
          <Text style={[styles.tabText, selectedTab === 'Humidity' && styles.activeTabText]}>
            Humidity
          </Text>
        </TouchableOpacity>
      </View>

      <LineChart
        data={selectedTab === 'Temperature' ? dataTemp : dataHumi}
        color={'#177AD5'}
        thickness={5}
        dataPointsColor={'red'}
        spacing={20}
        initialSpacing={0}
        height={200}
        // curveType={CurveType.CUBIC}
        // curvature={0}
        // animateOnDataChange
        // animationDuration={500}
        // onDataChangeAnimationDuration={300}
        yAxisOffset={selectedTab === 'Temperature' ? minTemp *0.9 : minHumi * 0.9}
        // chartConfig={{
        //   backgroundColor: '#ffffff',
        //   backgroundGradientFrom: '#ffffff',
        //   backgroundGradientTo: '#ffffff',
        //   decimalPlaces: 1,
        //   color: (opacity = 1) => `rgba(0, 0, 255, ${opacity})`,
        //   labelColor: (opacity = 1) => `rgba(0, 0, 0, ${opacity})`,
        //   style: { borderRadius: 16 },
        // }}
        
      />

    </ScrollView>
  );
};

const styles = StyleSheet.create({
  container: {
    flexGrow: 1,
    alignItems: 'center',
    backgroundColor: '#F5F8FE',
  },
  // headerContainer: {
  //   flexDirection: 'row',
  //   justifyContent: 'space-between', // Căn chỉnh tiêu đề và nút refresh
  //   width: '90%', // Chiếm 90% chiều rộng màn hình
  //   marginBottom: 10,
  //   alignContent: 'center',
  // },
  header: {
    width: '100%', // Chiếm 90% chiều rộng màn hình
    marginBottom: 10,
    fontSize: 23,
    fontWeight: 'bold',
    alignSelf: 'center',
    justifyContent: 'center',
    textAlign: 'center',
  },
  container1: {
    flexDirection: 'row',
    width: '100%',
    marginBottom: 10,
    marginTop: 10,
    justifyContent:'flex-start',
    alignContent: 'flex-end',
    overflow: 'hidden',
  },
  cardContent: {
    flexDirection: 'row', // Đặt các phần tử theo chiều ngang
    alignItems: 'flex-end', // Căn giữa các phần tử theo chiều dọc
    justifyContent: 'flex-start', // Căn trái các phần tử
  },
  cardContainer: {
    flexDirection: 'column',
    justifyContent: 'space-between',
    alignContent: 'center',
    width: '50%',
    marginTop: 45,
    marginBottom: 45,
    marginLeft: 15,
  },
  card: {
    backgroundColor: '#ffffff',
    borderRadius: 20,
    shadowColor: '#000',
    shadowOpacity: 0.1,
    shadowRadius: 5,
    elevation: 3,
    marginTop: 10,
    marginBottom: 10,
    alignItems: 'center',
    justifyContent: 'center',
    paddingBlockEnd:10,
    paddingBlockStart:10,
    paddingStart:1,
  },
  cardValue: {
    fontSize: 40,
    fontWeight: 'bold',
    textAlign: 'center',
  },
  cardStatus: {
    fontSize: 20,
    color: '#666',
    marginTop: 5,
    textAlign: 'center',
  },
  tabContainer: {
    flexDirection: 'row',
    position: 'relative',
    width: '90%',
    height: 50,
    backgroundColor: '#e0e0e0', // Màu nền của container
    borderRadius: 25,
    overflow: 'hidden',
  },
  tab: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    // backgroundColor: 'green',
  },
  activeTab: {
    backgroundColor: 'transparent', // Giữ nền trong suốt cho active tab
  },
  tabText: {
    fontSize: 16,
    color: '#000',
  },
  chart: {
    borderRadius: 10,
    marginVertical: 20,
  },
  footerNav: {
    flexDirection: 'row',
    justifyContent: 'space-around',
    width: '100%',
    paddingVertical: 10,
    borderTopWidth: 1,
    borderColor: '#ccc',
    backgroundColor: '#fff',
  },
  navItem: {
    fontSize: 16,
    color: '#007AFF',
  },
  icon: {
    alignSelf: 'center',
  },
  activeTabText: {
    fontWeight: 'bold',
    color: '#000',
    textAlign: 'center',
  },
  sliderContainer: {
    position: 'relative',
    width: '100%',
    height: 4,
    backgroundColor: '#e0e0e0',
    marginTop: 5,
  },
  slider: {
    position: 'absolute',
    width: '50%',
    height: '100%',
    backgroundColor: '#007AFF',
    borderRadius: 2,
  },
  indicator: {
    position: 'absolute',
    width: '43%',
    height: '90%',
    backgroundColor: '#fff',
    borderRadius: 25, // Góc bo tròn
    elevation: 2, // Hiệu ứng đổ bóng
    top: '5%', // Canh giữa theo chiều dọc
    left: '1%', // Căn chỉnh nhỏ hơn để cân đối trong tab
    justifyContent: 'center', // Canh giữa nội dung theo chiều dọc
    alignItems: 'center', // Canh giữa nội dung theo chiều ngang
    textAlign: 'center',
  },
  refreshButton: {
    backgroundColor: '#f0f0f0',
    padding: 10,
    borderRadius: 50,
    elevation: 3,
  },
});

export default App;
