import {
  Box,
  Button,
  ButtonGroup,
  Flex,
  HStack,
  IconButton,
  SkeletonText,
  Text,
} from '@chakra-ui/react'

import { FaLocationArrow } from 'react-icons/fa'
import { FiRefreshCw } from 'react-icons/fi'

import {
  useJsApiLoader,
  GoogleMap,
  Marker,
  HeatmapLayer,
} from '@react-google-maps/api'

import { useRef, useState, useEffect } from 'react'

import axios from './Utils/axios_main'
import "./App.css"


const center = { latitude: 44.942252244435046, longitude: 26.01893741152837 }
const libs = ['places', 'visualization'];

function App() {

  const [map, setMap] = useState(/** @type google.maps.Map */(null))
  const [currentMarker, setCurrentMarker] = useState({ latitude: center.latitude, longitude: center.longitude })
  const [markers, setMarkers] = useState([]);
  const [refreshedAt, setrefreshedAt] = useState(Date());

  // const [latitude, setLatitude] = useState(center.lat);
  // const [longitude, setLongitude] = useState(center.lng);


  useEffect(async function () {

    try {
      // const response = await axios.get('/locations', 
      //   {
      //     headers: {
      //       'Content-Type': 'application/json',
      //       'token': auth.token
      //     },
      //     //withCredentials: true
      //   }

      // )

      // setMarkers(response.data.locations);
      // setMarkers(markers.slice())
      setMarkers(heatmapData)
    } catch (err) {
      console.log(err)

      if (!err?.response)
        console.log(err?.response)

    }
  }, []);

  const heatmapData = [
    { latitude: 44.94398456988681, longitude: 25.991712629305137 },
    { latitude: 44.94398456988681, longitude: 25.991712629305137 },
    { latitude: 44.94398456988681, longitude: 25.991712629305137 },
    { latitude: 44.94398456988681, longitude: 25.991712629305137 },
    { latitude: 44.94398456988681, longitude: 25.991712629305137 },

    { latitude: 44.929100120492066, longitude: 26.010187384319053 },
    { latitude: 44.929100120492066, longitude: 26.010187384319053 },
    { latitude: 44.929100120492066, longitude: 26.010187384319053 },

    { latitude: 44.95149758371722, longitude: 26.023150332757496 },
    { latitude: 44.95149758371722, longitude: 26.023150332757496 },
    { latitude: 44.95149758371722, longitude: 26.023150332757496 },
    { latitude: 44.95149758371722, longitude: 26.023150332757496 },

    { latitude: 44.95510925951168, longitude: 26.013963990485387 },
  ]

  const handleMapclick = (e) => {

    setCurrentMarker({
      latitude: e.latLng.lat(),
      longitude: e.latLng.lng()
    })
  }

  const gradient = [
    'rgba(16, 52, 166, 0)',
    'rgba(65, 47, 136, 1)',
    'rgba(255, 206, 3, 1)',
    'rgba(253, 97,  4, 1)',
    'rgba(211, 33, 45, 1)',
    'rgba(246, 45, 45, 1)',
    'rgba(240, 5, 5, 1)'
    // 'rgba(0, 0, 191, 1)',
    // 'rgba(0, 0, 159, 1)',
    // 'rgba(0, 0, 127, 1)',
    // 'rgba(63, 0, 91, 1)',
    // 'rgba(127, 0, 63, 1)',
    // 'rgba(191, 0, 31, 1)',
    // 'rgba(255, 0, 0, 1)'
  ]

  const addMarker = async () => {
    console.log(markers);
    var newMArkers = markers.slice();
    newMArkers.push({
      latitude: currentMarker.latitude,
      longitude: currentMarker.longitude
    })

    setMarkers(newMArkers);
  }

  const refreshPage = () => {
    setMarkers([]);
    setrefreshedAt(Date());
  }

  const { isLoaded } = useJsApiLoader({
    googleMapsApiKey: process.env.REACT_APP_GOOGLE_MAPS_API_KEY,
    libraries: libs,
  })

  if (!isLoaded) {
    return <SkeletonText />
  }

  return (
    <Flex
      position='relative'
      flexDirection='column'
      alignItems='center'
      h='100vh'
      w='100vw'
    >
      <Box position='absolute' left={0} top={0} h='100%' w='100%'>
        {/* Google Map Box */}
        <GoogleMap
          center={{ lat: center.latitude, lng: center.longitude }}
          zoom={16}
          mapContainerStyle={{ width: '100%', height: '100%' }}
          options={{
            zoomControl: false,
            streetViewControl: false,
            mapTypeControl: false,
            fullscreenControl: false,
          }}
          onClick={e => handleMapclick(e)}
          onLoad={map => setMap(map)}

        >
          <HeatmapLayer
            data={markers.map((marker) => (
              new window.google.maps.LatLng(marker.latitude, marker.longitude)
            ))}
            options={{
              radius: 30,
              gradient: gradient,
            }}
          >
          </HeatmapLayer>
          <Marker position={{ lat: currentMarker.latitude, lng: currentMarker.longitude }} />
          {markers.map((marker, index) => {

            return <Marker key={index} position={{ lat: marker.latitude, lng: marker.longitude }} onClick />

          })}
        </GoogleMap>
      </Box>
      <Box
        p={4}
        borderRadius='lg'
        m={4}
        bgColor='white'
        shadow='base'
        minW='container.md'
        zIndex='1'
      >
        <HStack pacing={2} justifyContent='flex-start'>
          <IconButton
            aria-label='refresh page'
            icon={<FiRefreshCw />}
            isRound
            onClick={refreshPage}
          />
          <Text> Last refreshed at: {refreshedAt}</Text>
        </HStack>
      </Box>
    </Flex>
  )
}

export default App
