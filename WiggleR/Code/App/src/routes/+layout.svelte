<script>
  import { goto } from '$app/navigation'
	import { onMount } from 'svelte';

  onMount(async () => {
    await import('@kor-ui/kor');
    await import('@kor-ui/kor/kor-styles.css');
	})
</script>

<kor-page>
  <kor-app-bar
    slot="top"
    label="WiggleR"
  >
    <kor-tabs>
      <kor-tab-item on:click={() => goto('/')} label="Dashboard" active></kor-tab-item>
      <kor-tab-item on:click={() => goto('/timelapse')} label="Timelapse"></kor-tab-item>
      <kor-tab-item on:click={() => goto('/timeline')} label="Timeline"></kor-tab-item>
      <kor-tab-item on:click={() => goto('/test')} label="Test"></kor-tab-item>
    </kor-tabs>
  </kor-app-bar>
  <kor-pane
    slot="left"
    icon="extension"
    label="Filters"
  ></kor-pane>

  <slot />
</kor-page>