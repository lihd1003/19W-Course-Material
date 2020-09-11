<script type="module">
  import * as THREE from 'https://unpkg.com/three/build/three.module.js';
  import { OrbitControls } from 'https://unpkg.com/three/examples/jsm/controls/OrbitControls.js';
  import { OBJLoader } from 'https://unpkg.com/three/examples/jsm/loaders/OBJLoader.js';

  const OBJViewer = (canvas, obj_file, texture_img) => {
    const renderer = new THREE.WebGLRenderer({ canvas });
    const camera = new THREE.PerspectiveCamera(45, 2, 0.1, 100);
    camera.position.set(0, 0, 5);

    const controls = new OrbitControls(camera, canvas);
    controls.target.set(0, 0, 0);
    controls.update();

    const scene = new THREE.Scene();
    scene.background = new THREE.Color('white');

    const textureLoader = new THREE.TextureLoader();
    const texture = textureLoader.load(texture_img)

    {
      const skyColor = 0xB1E1FF;  // light blue
      const groundColor = 0xB97A20;  // brownish orange
      const intensity = 1;
      const light = new THREE.HemisphereLight(skyColor, groundColor, intensity);
      scene.add(light);
    }

    {
      const color = 0xFFFFFF;
      const intensity = 1;
      const light = new THREE.DirectionalLight(color, intensity);
      light.position.set(0, 10, 0);
      light.target.position.set(-5, 0, 0);
      scene.add(light);
      scene.add(light.target);
    }


    const objLoader = new OBJLoader();
    objLoader.load(obj_file, (root) => {
      root.traverse(function (child) {
        if (child.isMesh) child.material.map = texture;
      });
      scene.add(root);
    });

    function resizeRendererToDisplaySize(renderer) {
      const canvas = renderer.domElement;
      const width = canvas.clientWidth;
      const height = canvas.clientHeight;
      const needResize = canvas.width !== width || canvas.height !== height;
      if (needResize) {
        renderer.setSize(width, height, false);
      }
      return needResize;
    }

    function render() {

      if (resizeRendererToDisplaySize(renderer)) {
        const canvas = renderer.domElement;
        camera.aspect = canvas.clientWidth / canvas.clientHeight;
        camera.updateProjectionMatrix();
      }

      renderer.render(scene, camera);

      requestAnimationFrame(render);
    }

    requestAnimationFrame(render);
  }
{% for viewer in OBJViewers %}
  OBJViewer(document.querySelector("#{{ viewer.name }}"), "{{ viewer.obj }}", "{{ viewer.texture }}");
{% endfor %} 
</script>