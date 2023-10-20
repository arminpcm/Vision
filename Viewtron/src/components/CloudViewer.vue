<template>
    <div ref="container" class="viewerContainer">
      <!-- Your component content -->
    </div>
</template>
  
<script>
import { defineComponent } from 'vue';
import * as THREE from 'three';
import { PCDLoader } from 'three/addons/loaders/PCDLoader.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';

export default defineComponent({
methods: {
    initScene() {
    const scene = new THREE.Scene();
    const ambientLight = new THREE.AmbientLight(0x35424a);
    scene.add(ambientLight);
    return scene;
    },
    initRenderer() {
    const renderer = new THREE.WebGLRenderer();
    renderer.setClearColor(0x35424a);
    const width = this.$parent.$el.offsetWidth;
    const height = this.$parent.$el.offsetHeight;
    renderer.setSize(width, height);
    this.$el.appendChild(renderer.domElement);
    return renderer;
    },
    createControls() {
    const controls = new OrbitControls(this.camera, this.renderer.domElement);
    controls.target.set(0, 0, 0);
    controls.enableDamping = true;
    controls.tick = () => controls.update();
    controls.addEventListener('change', () => {
        this.renderer.render(this.scene, this.camera);
    });
    return controls;
    },
    loadPointCloud(scene, camera, renderer) {
        const loader = new PCDLoader();
        loader.load('/background.pcd', (points) => {
            scene.add(points);
            this.camera.position.set(0.36, 0.34, 0.6);
            this.camera.rotation.set(-0.35, 0.37, 0.13);
            // this.camera.lookAt(0, 0, 0);
            this.controls.saveState();

            const animate = () => {
            requestAnimationFrame(animate);
            renderer.render(scene, this.camera);
            };

            animate();
        });
        },
        handleKeyDown(event) {
        if (event.key === 'r') {
            this.controls.reset();
        }
    },
},
mounted() {
    this.scene = this.initScene();
    this.renderer = this.initRenderer();
    const width = this.$parent.$el.offsetWidth;
    const height = this.$parent.$el.offsetHeight;
    this.camera = new THREE.PerspectiveCamera(15, width / height, 0.01, 100);
    this.controls = this.createControls(this.camera, this.renderer.domElement);
    this.loadPointCloud(this.scene, this.camera, this.renderer);
    window.addEventListener('keydown', this.handleKeyDown);
},
beforeUnmount() {
    window.removeEventListener('keydown', this.handleKeyDown);
},
});
</script>

<style>
  .viewerContainer {
    width: 100%;
    height: 100%;
  }
</style>
  