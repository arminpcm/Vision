<template>
  <div class="viewerContainer"></div>
</template>

<script>
import { defineComponent } from 'vue';
import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';

export default defineComponent({
  mounted() {
    const textureLoader = new THREE.TextureLoader();
    const texture = textureLoader.load('/background.png', () => {
      const containerWidth = this.$parent.$el.offsetWidth;
      const containerHeight = this.$parent.$el.offsetHeight;

      const planeWidth = texture.image.width;
      const planeHeight = texture.image.height;

      // Calculate width and height ratios
      const widthRatio = containerWidth / planeWidth;
      const heightRatio = containerHeight / planeHeight;

      // Choose the smaller ratio as the scaling factor
      const scaleFactor = (widthRatio < heightRatio) ? widthRatio : heightRatio;

      // Scale rectangle B
      const scaledPlaneHeight = planeHeight * scaleFactor;
      const scaledPlaneWidth = planeWidth * scaleFactor;

      this.camera = new THREE.OrthographicCamera(
        scaledPlaneWidth / -2,
        scaledPlaneWidth / 2,
        scaledPlaneHeight / 2,
        scaledPlaneHeight / -2,
        -100,
        100
      );

      const scene = new THREE.Scene();
      const ambientLight = new THREE.AmbientLight(0x35424a);
      scene.add(ambientLight);
      this.camera.position.set(0, 0, 1);
      this.camera.lookAt(scene.position);

      const renderer = new THREE.WebGLRenderer();
      renderer.setClearColor(0x35424a);
      renderer.setSize(scaledPlaneWidth, scaledPlaneHeight);
      this.$el.appendChild(renderer.domElement);

      this.controls = new OrbitControls(this.camera, renderer.domElement);
      this.controls.enableRotate = false;
      this.controls.enableZoom = true;
      this.controls.enablePan = true;
      this.controls.saveState();

      const planeGeometry = new THREE.PlaneGeometry(scaledPlaneWidth, scaledPlaneHeight);
      const planeMaterial = new THREE.MeshBasicMaterial({ map: texture, side: THREE.DoubleSide });
      const plane = new THREE.Mesh(planeGeometry, planeMaterial);
      plane.position.set(0, 0, 1);
      scene.add(plane);

      const animate = () => {
        requestAnimationFrame(animate);
        this.controls.update();
        renderer.render(scene, this.camera);
      };

      animate();
    });

    window.addEventListener('keydown', this.handleKeyDown);
  },
  beforeUnmount() {
    window.removeEventListener('keydown', this.handleKeyDown);
  },
  methods: {
    handleKeyDown(event) {
      if (event.key === 'r') {
        this.controls.reset();
      }
    }
  }
});
</script>

<style>
.viewerContainer {
  width: 100%;
  height: 100%;
  overflow: hidden;
}
</style>
