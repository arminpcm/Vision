<template>
    <div :class="['sidebar', { 'sidebar-open': isOpen }]">
      <!-- Sidebar content goes here -->
      <div class="sidebar-content">
        <!-- Sidebar content -->
      </div>
    </div>
  </template>
  
  <script>
  export default {
    data() {
      return {
        isOpen: false,
      };
    },
    mounted() {
      // Add a keyboard event listener for the 'b' key
      window.addEventListener('keydown', this.handleKeyDown);
    },
    methods: {
      // Method to handle keyboard keydown event
      handleKeyDown(event) {
        if (event.key === 'b') {
          this.isOpen = !this.isOpen;
          this.$emit('sidebarToggled', this.isOpen); // Emit the sidebar state
        }
      },
    },
    beforeUnmount() {
      // Remove the event listener when the component is destroyed
      window.removeEventListener('keydown', this.handleKeyDown);
    },
  };
  </script>
  
  <style scoped>
  /* Sidebar-specific styles */
.sidebar {
    width: 0;
    height: 100vh;
    background-color: #000e105b; /* Set your desired background color for the sidebar */
    position: fixed;
    top: 0;
    left: 0;
    z-index: 1000; /* Ensure the sidebar appears above the overlay */
    overflow-x: hidden; /* Hide horizontal scrollbar */
    transition: width 0.3s; /* Smoothly transition the sidebar width */
  }
  
  .sidebar-open {
    width: 250px; /* Set your desired width for the open sidebar */
  }
  </style>
  