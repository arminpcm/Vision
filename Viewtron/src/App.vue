<template>
  <div id="app" :class="{ 'sidebar-open': sidebarOpen }">
    <div>
      <Sidebar />
    </div>
    <!-- Overlay element -->
    <div class="overlay" v-if="sidebarOpen" @click="closeSidebar"></div>
    <div class="content-container">
      <div class="viewer-container">
        <div class="cloud-viewer">
          <h1>Cloud Viewer App</h1>
          <CloudViewer/>
        </div>
        <div class="image-viewer">
          <h1>Image Viewer App</h1>
          <ImageViewer/>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import ImageViewer from './components/ImageViewer.vue';
import CloudViewer from './components/CloudViewer.vue';
import Sidebar from './components/Sidebar.vue';
import { ref, watch } from 'vue';

const sidebarOpen = ref(false);

const updateSidebarState = (isOpen) => {
  sidebarOpen.value = isOpen;
};

// Listen for 'b' keypress to toggle sidebar
window.addEventListener('keydown', (event) => {
  if (event.key === 'b') {
    sidebarOpen.value = !sidebarOpen.value;
  }
});

// Watch for changes in sidebarOpen and emit the event accordingly
watch(sidebarOpen, (newValue) => {
  updateSidebarState(newValue);
});
</script>

<style scoped>
.sidebar-open .content-container {
  margin-left: 250px; /* Adjust this value based on your sidebar width */
}
/* Adjust the content position when sidebar is open */
.content-container.shifted {
  position: relative;
  left: 250px; /* Adjust this value based on your sidebar width */
}
.overlay {
  position: fixed;
  top: 0;
  left: 0;
  width: 100vw;
  height: 100vh;
  background-color: rgba(0, 0, 0, 0.5); /* Semi-transparent black color */
  z-index: 999; /* Ensure the overlay appears above other content */
  transition: opacity 0.3s; /* Smoothly transition the overlay opacity */
}

body, #app {
  margin: 0;
  padding: 0;
}

.content-container {
  width: 100%;
  max-width: 100vw; /* Ensure content doesn't exceed viewport width */
  height: 100%;
}

.viewer-container {
  display: flex;
}

.image-viewer {
  box-sizing: border-box; /* Include padding and border in element's total width */
  flex: 0 0 50%;
  padding: 20px;
  background-color: #f0f0f0;
  border: 1px solid #ccc;
  border-radius: 5px;
  margin: 10px;
  max-width: calc(50% - 20px); /* Set max-width to prevent exceeding 50% */
}

.cloud-viewer {
  box-sizing: border-box; /* Include padding and border in element's total width */
  flex: 0 0 50%;
  padding: 20px;
  background-color: #f0f0f0;
  border: 1px solid #ccc;
  border-radius: 5px;
  margin: 10px;
  max-width: calc(50% - 20px); /* Set max-width to prevent exceeding 50% */
}

</style>
