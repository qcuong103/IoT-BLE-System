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
      humidity: 50.0, 
      temperature: 25.0, 
      time: new Date().toLocaleTimeString(), 
      })
  ]); 
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
  };
  
  const animatedIndicatorStyle = useAnimatedStyle(() => ({
    transform: [{ translateX: indicatorPosition.value }],
  }));

  useEffect(() => {
      const sensorDataRef = ref(database, 'sensorData/');
  

      const unsubscribe = onValue(sensorDataRef, (snapshot) => {
        if (snapshot.exists()) {
          const data = snapshot.val();
          if (data) {
            setTemperature(data.temperature); 
            setHumidity(data.humidity);       
            if (data.temperature < minTemp) {
              setMinTemp(data.temperature);
            }
            if (data.humidity < minHumi) {
              setMinHumi(data.humidity);
            }
          }
          const newData = {
            time: data.time ?? new Date().toLocaleTimeString(), 
            temperature: data.temperature,
            humidity: data.humidity,
          };
          setSensorData((prevData) => {
            const updatedData = [...prevData, newData];
            if (updatedData.length > 30) {
              updatedData.shift();
            }
            return updatedData;
              });
            } else {
              console.log("No data available");
        
        }
      });
  
      return () => unsubscribe();
    }, []); 

    const chartData = {
      temperature: sensorData.map(item => item.temperature),
      humidity: sensorData.map(item => item.humidity),
    };
    // Dữ liệu của biểu đồ
    const dataTemp = chartData.temperature.map(value => ({ value }));  
    const dataHumi = chartData.humidity.map(value => ({ value }));  
  return (
    <ScrollView contentContainerStyle={styles.container}>
      <Text style={styles.header}>Temperature and Humidity</Text>
      <ImageBackground
      source={require('./assets/bg.png')} 
      style={styles.container1} 
      resizeMode="cover" 
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
        yAxisOffset={selectedTab === 'Temperature' ? minTemp *0.9 : minHumi * 0.9}
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
  header: {
    width: '100%', 
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
    flexDirection: 'row', 
    alignItems: 'flex-end', 
    justifyContent: 'flex-start', 
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
    backgroundColor: '#e0e0e0', 
    borderRadius: 25,
    overflow: 'hidden',
  },
  tab: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  activeTab: {
    backgroundColor: 'transparent',
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
    borderRadius: 25,
    elevation: 2, 
    top: '5%',
    left: '1%',
    justifyContent: 'center',
    alignItems: 'center', 
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
